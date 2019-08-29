/* nbbqdrv.c

   June 27, 2003  H.Baba
*/


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/config.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "bbmodules.h"
#include "nbbqctrl.h"
#include "nbbqdrv.h"
#include "nbbqio.h"
#include "bbuniverse.h"

#if LINUX_VERSION_CODE >=  0x020410
MODULE_LICENSE("GPL");
#endif

#include "./common.h"

#ifdef BBRL
#include "bbrl.c"
#endif

#ifdef RDTCSDEF
volatile unsigned long long int RDTSC(void)
{
   unsigned int h,l;
  
   /* read Pentium cycle counter */
      __asm__(".byte 0x0f,0x31"
	      :"=a" (l),
	                 "=d" (h));
  
    return ((unsigned long long int)h<<32)|l;
}
unsigned long long st,en;
unsigned long long dst,den;
#endif

#include "univfunc.c"
//#include "univdma.c"
#include "v1290.h"
#include "sis3800.c"
#include "sis3300.c"
#include "rpv130.c"
#include "startup.c"
#include "evt.c"
#include "stop.c"
#include "sca.c"
#include "clear.c"


vme_master_handle_t *master_p;
vme_interrupt_handle_t interrupt;

static int nbbqdrv_major = NBBQ_MAJOR;
static const char *nbbqdrv_name = "nbbqdrv";
static char *irq_name = "irq_nbbq";

ssize_t nbbqdrv_read(struct file *file,char *buff,size_t count,loff_t *pos);
static int nbbqdrv_open(struct inode* inode, struct file* filep);
static int nbbqdrv_release(struct inode* inode, struct file* filep);
static int nbbqdrv_ioctl(struct inode *inode, struct file *filep,
			 unsigned int cmd, unsigned long arg);
static unsigned int nbbqdrv_poll(struct file *file,poll_table *wait);
static void nbbqdrv_interrupt(int irq,void *dev_id,struct pt_regs* regs);

static int girq,chkbuff,chkblk;
#if LINUX_VERSION_CODE >=  0x020400
wait_queue_head_t nbbq_wait_queue;
#else
struct wait_queue *nbbq_wait_queue = NULL;
#endif

#if LINUX_VERSION_CODE >=  0x020400
static struct file_operations nbbqdrv_fops = {
  read: nbbqdrv_read,
  poll: nbbqdrv_poll,
  ioctl: nbbqdrv_ioctl,
  open: nbbqdrv_open,
  release: nbbqdrv_release,
};
#else
static struct file_operations nbbqdrv_fops = {
  NULL,             // loff_t  llseek
  nbbqdrv_read,     // ssize_t read
  NULL,             // ssize_t write
  NULL,             // int     readdir
  nbbqdrv_poll,     // uint    poll
  nbbqdrv_ioctl,    // int     ioctl
  NULL,             // int     mmap
  nbbqdrv_open,     // int     open
  NULL,             // int     flush
  nbbqdrv_release,  // int     release
};
#endif

int init_module(void){
  int ret;

  ret = register_chrdev(nbbqdrv_major,nbbqdrv_name,&nbbqdrv_fops);
  if(ret < 0){
    return ret;
  }
  if(nbbqdrv_major == 0) {
    nbbqdrv_major = ret;
  }

  vme_init(&bus);
  vme_interrupt_irq(bus, &girq);
  
  chkbuff = 0;

#if LINUX_VERSION_CODE >=  0x020400
  init_waitqueue_head(&nbbq_wait_queue);
#endif

  printk("%s : nbbqdrv was installed (irq %d).\n",nbbqdrv_name,girq);

  return 0;
}

void cleanup_module(void){
  vme_interrupt_enable(bus, interrupt);
  unregister_chrdev(nbbqdrv_major,nbbqdrv_name);
  vme_term(bus);
  printk("%s: nbbqdrv was unregistered.\n", nbbqdrv_name);
}

static int nbbqdrv_open(struct inode* inode, struct file* filep){
  int ret;

  if(MOD_IN_USE){
    return -EBUSY;
  }

  chkblk = 0;

  init_mem();

  ret = request_irq(girq,nbbqdrv_interrupt,SA_SHIRQ,nbbqdrv_name,irq_name);
  vme_interrupt_attach(bus, &interrupt, INTLEVEL, INTVEC,
		       VME_INTERRUPT_RESERVE, NULL);
  vme_interrupt_enable(bus, interrupt);
  
  MOD_INC_USE_COUNT;

  startup();

  return 0;
}

static int nbbqdrv_release(struct inode* inode, struct file* filep){

  MOD_DEC_USE_COUNT;

  return 0;
}

ssize_t nbbqdrv_read(struct file *file,char *buff,size_t count,loff_t *pos){
  chkbuff = 0;

  copy_to_user(buff,data,sizeof(data));

  return 1;
}


static int nbbqdrv_ioctl(struct inode* inode, struct file *filep,
			 unsigned int cmd, unsigned long arg){

  char flag;

  switch(cmd){
  case NBBQ_STOP:
    vme_interrupt_release(bus, interrupt);
    free_irq(girq,irq_name);
    if(chkblk == 1){
      end_block();
      sca();
      flag = 1;
      copy_to_user((void *)arg,&flag,1);
      chkbuff = 1;
      chkblk = 0;
    }else{
      flag = 0;
      copy_to_user((void *)arg,&flag,1);
    }
    stop();
    break;
  case NBBQ_EI:
    clear();
    vme_interrupt_enable(bus, interrupt);
    vme_interrupt_clear(bus, interrupt);
    break;
  }

  return 1;
}

static unsigned int nbbqdrv_poll(struct file *file,poll_table *wait){
  poll_wait(file,&nbbq_wait_queue,wait);
  if(chkbuff == 1){
    return POLLIN;
  }else{
    return 0;
  }
}

static void nbbqdrv_interrupt(int irq, void* dev_id, struct pt_regs* regs){
  int vector;

  if(vme_interrupt_asserted(bus, interrupt)){
    vme_interrupt_vector(bus, interrupt, &vector);
    vme_interrupt_disable(bus, interrupt);

    if(chkblk == 0){
      init_block();
      chkblk = 1;
    }
    evt();
    if(end_event() > MAXBUFF){
      end_block();
      sca();
      chkbuff = 1;
      chkblk = 0;
    }else{
      clear();
      vme_interrupt_enable(bus, interrupt);
      vme_interrupt_clear(bus, interrupt);
    }
  }

  wake_up_interruptible(&nbbq_wait_queue);
}
