/* babirl/babidrv/babildrv.c
 * 
 *
 * DAQ driver for normal Linux with babirl
 * for SVA041 2.6.34.1
 *
 * Hidetada Baba (RIKEN)
 * baba@ribf.riken.jp
 *
*/

#define LINUX_VERSION_CODE 0x020634

/*
  #include <linux/version.h>
  #if LINUX_VERSION_CODE >=  0x020600
  #if defined(USE_MODVERSIONS) && USE_MODVERSIONS
  #  define MODVERSIONS
  #  include <config/modversions.h>
  #endif
  #endif
*/
 
#include <linux/module.h>
#include <linux/kernel.h>
#if LINUX_VERSION_CODE >=  0x020600
#include <linux/init.h>
#include <linux/interrupt.h>
#endif
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/sched.h>
#if LINUX_VERSION_CODE <  0x020600
#include <linux/config.h>
#endif
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <unistd.h>

#include "bbuniverse.h"

#include "bbmodules.h"
#include "babilctrl.h"
#include "babildrv.h"
#include "babilio.h"

static volatile int mpflag=0;

#if LINUX_VERSION_CODE >=  0x020410
MODULE_LICENSE("GPL");
#endif
#if LINUX_VERSION_CODE >=  0x020600
MODULE_AUTHOR("Hidetada Baba");
#ifndef MOD_IN_USE
#define MOD_IN_USE module_refcount(THIS_MODULE)
#endif
#ifndef MOD_INC_USE_COUNT
#define MOD_INC_USE_COUNT
#endif
#ifndef MOD_DEC_USE_COUNT
#define MOD_DEC_USE_COUNT
#endif
#endif


//#define DEBUG

#ifdef DEBUG
#define DB(x) x
#else
#define DB(x)
#endif

#include "babirldrvdef.h"
#include "babirldrvridf.c"

#ifdef CAMAC
//#include "babirldrvcamac.c"
#include "camac.c"
#endif

#ifdef VME
#include "univfunc.c"
#include "babirldrvcaen.c"
#include "v1190.h"
#include "madc32.c"
#include "mqdc32.c"
#include "rpv130.c"
#include "tmb2.c"
#include "vlupomap.c"
#endif

#ifdef CCNET
#include "ccnetevt.c"
#include "usrccnetevt.c"
#endif

#include "sca.c"
#include "startup.c"
#include "evt.c"
#include "stop.c"
#include "clear.c"


// for vmeuniv
//vme_master_handle_t *master_p;


static int babildrv_major = BABILDRV_MAJOR;
static const char *babildrv_name = "babildrv";
#if LINUX_VERSION_CODE >= 0x020600
static int dev_id;
#else
static char *irq_name = "irq_babil";
#endif


ssize_t babildrv_read(struct file *file,char *buff,size_t count,loff_t *pos);
static int babildrv_open(struct inode* inode, struct file* filep);
static int babildrv_release(struct inode* inode, struct file* filep);
static int babildrv_ioctl(struct inode *inode, struct file *filep,
			 unsigned int cmd, unsigned long arg);
static unsigned int babildrv_poll(struct file *file,poll_table *wait);
#if LINUX_VERSION_CODE >= 0x020600
static irqreturn_t babildrv_interrupt(int irq, void* dev_id, struct pt_regs* regs);
#else
static void babildrv_interrupt(int irq,void *dev_id,struct pt_regs* regs);
#endif

static int girq;
volatile int chkbuff, chkblk;
#if LINUX_VERSION_CODE >=  0x020400
wait_queue_head_t babil_wait_queue;
#else
struct wait_queue *babil_wait_queue = NULL;
#endif

#if LINUX_VERSION_CODE >=  0x020600
static struct file_operations babildrv_fops = {
  .read = babildrv_read,
  .poll = babildrv_poll,
  .ioctl = babildrv_ioctl,
  .open = babildrv_open,
  .release = babildrv_release,
};
#else
#if LINUX_VERSION_CODE >=  0x020400
static struct file_operations babildrv_fops = {
  read: babildrv_read,
  poll: babildrv_poll,
  ioctl: babildrv_ioctl,
  open: babildrv_open,
  release: babildrv_release,
};
#else
static struct file_operations babildrv_fops = {
  NULL,             // loff_t  llseek
  babildrv_read,     // ssize_t read
  NULL,             // ssize_t write
  NULL,             // int     readdir
  babildrv_poll,     // uint    poll
  babildrv_ioctl,    // int     ioctl
  NULL,             // int     mmap
  babildrv_open,     // int     open
  NULL,             // int     flush
  babildrv_release,  // int     release
};
#endif
#endif

#ifdef DBUFF
volatile static int stopflag = 0;
volatile int drn = 0;
#endif

#if LINUX_VERSION_CODE >= 0x020600
int babildrv_init_module(void){
#else
int init_module(void){
#endif
  int ret;

  ret = register_chrdev(babildrv_major,babildrv_name,&babildrv_fops);
  if(ret < 0){
    printk("%s : can't regist.\n",babildrv_name);
    return ret;
  }
  if(babildrv_major == 0) {
    babildrv_major = ret;
  }


  vme_init(&bus);
  vme_interrupt_irq(bus, &girq);

  map();

  chkbuff = 0;

#if LINUX_VERSION_CODE >=  0x020400
  init_waitqueue_head(&babil_wait_queue);
#endif


  vme_interrupt_clear2(INTLEVEL);
  vme_interrupt_disable2(INTLEVEL);

#if LINUX_VERSION_CODE >= 0x020618
  ret = request_irq(girq,(void *)babildrv_interrupt,IRQF_SHARED,babildrv_name,&dev_id);
#else
  ret = request_irq(girq,babildrv_interrupt,SA_SHIRQ,babildrv_name,irq_name);
#endif

  printk("%s : babildrv was installed (irq %d).\n",babildrv_name,girq);

  return 0;
}

#if LINUX_VERSION_CODE >= 0x020600
void babildrv_cleanup_module(void){
#else
void cleanup_module(void){
  unmap();

#endif
  vme_interrupt_clear2(INTLEVEL);
  vme_interrupt_disable2(INTLEVEL);

  vme_term(bus);
#if LINUX_VERSION_CODE >= 0x020600
  free_irq(girq,&dev_id);
#else
  free_irq(girq,irq_name);
#endif

  unregister_chrdev(babildrv_major,babildrv_name);
  printk("%s: babildrv was unregistered.\n", babildrv_name);
}

static int babildrv_open(struct inode* inode, struct file* filep){
  //int ret;

  if(MOD_IN_USE){
    return -EBUSY;
  }

  DB(printk("babildrv: open\n"));

  vme_interrupt_enable2(INTLEVEL);
  vme_interrupt_clear2(INTLEVEL);

  chkblk = 0;
  chkbuff = 0;
#ifdef DBUFF
  stopflag = 0;
  drn = 0;
#endif

  init_mem();

  MOD_INC_USE_COUNT;

  init_evtn();
  startup();

  return 0;
}

static int babildrv_release(struct inode* inode, struct file* filep){

  chkbuff = 0;
  vme_interrupt_clear2(INTLEVEL);
  vme_interrupt_disable2(INTLEVEL);

  MOD_DEC_USE_COUNT;

  return 0;
}

ssize_t babildrv_read(struct file *file,char *buff,size_t count,loff_t *pos){
  int len;
  unsigned long x;

#ifndef DBUFF
  len = mp * WORDSIZE;
  chkbuff = 0;
  x = copy_to_user(buff, data, len);
#else
  if(drn == 0){
    len = mpa * WORDSIZE;
  }else{
    len = mpb * WORDSIZE;
  }
  x = copy_to_user(buff,(char *)(databuff+EB_EFBLOCK_BUFFSIZE*drn),len);
  drn++;
  if(drn == 2) drn = 0;

  if(chkbuff == 2){
    chkbuff--;
    if(chkbuff < 0){
      chkbuff = 0;
    }
    clear();
    if(stopflag == 0){
      vme_interrupt_clear2(INTLEVEL);
      vme_interrupt_enable2(INTLEVEL);
    }
    if(stopflag == 0){
    }
  }else{
    chkbuff--;
    if(chkbuff < 0){
      chkbuff = 0;
    }
  }
#endif

  return 1;
}


static int babildrv_ioctl(struct inode* inode, struct file *filep,
			 unsigned int cmd, unsigned long arg){
  char flag;
  int len;
  unsigned long x;

  DB(printk("babildrv: ioctl cmd=%d\n", cmd));

  switch(cmd){
  case BABIL_STOP:
  case BABIL_STOPB:
#ifdef DBUFF
    stopflag = 1;
#endif
    if(chkblk == 1){
      sca();
      end_block();
#ifndef DBUFF
      flag = 1;
#else
      chkbuff++;
      flag = chkbuff;
      chmem();
#endif
      x = copy_to_user((void *)arg,&flag,1);
#ifndef DBUFF
      chkbuff = 1;
#endif
      chkblk = 0;
    }else{
      flag = 0;
      x = copy_to_user((void *)arg, &flag, 1);
    }
    stop();
    vme_interrupt_disable2(INTLEVEL);
    vme_interrupt_clear2(INTLEVEL);
    break;
  case BABIL_EI:
#ifndef DBUFF
    clear();
    vme_interrupt_enable2(INTLEVEL);
    vme_interrupt_clear2(INTLEVEL);
#endif
    break;
  case BABIL_LEN:
#ifndef DBUFF
    len = mp * WORDSIZE;
#else
    if(drn == 0){
      len = mpa * WORDSIZE;
    }else{
      len = mpb * WORDSIZE;
    }
#endif
    x = copy_to_user((void *)arg, (char *)&len, 4);
    break;
  case BABIL_EVTN:
    len = (int)get_evtn();
    x = copy_to_user((void *)arg, (char *)&len, 4);
    break;
  case BABIL_DBUFF:
#ifdef DBUFF
    len = chkbuff;
#else
    len = 1;
#endif
    x = copy_to_user((void *)arg, (char *)&len, 4);
    break;
  }

  return 1;
}

static unsigned int babildrv_poll(struct file *file,poll_table *wait){
  poll_wait(file, &babil_wait_queue,wait);
  if(chkbuff > 0){
    return POLLIN;
  }else{
    return 0;
  }
}

#if LINUX_VERSION_CODE >= 0x020600
static irqreturn_t babildrv_interrupt(int irq, void* dev_id, struct pt_regs* regs){
#else
static void babildrv_interrupt(int irq, void* dev_id, struct pt_regs* regs){
#endif
  int vector;
  
  DB(printk("babildrv: interrupt\n"));

  if(dev_id == NULL){
    printk("babildrv: interrupt no devid\n");
    return IRQ_NONE;
  }
  
  if(vme_interrupt_asserted2(INTLEVEL)){
    vme_interrupt_vector2(INTLEVEL, &vector);
    vme_interrupt_clear2(INTLEVEL);
    vme_interrupt_disable2(INTLEVEL);
    
    if(chkblk == 0){
      init_block();
      chkblk = 1;
    }
    
    evt();
#ifndef TIMESTAMPEF
    if(end_event() > MAXBUFF)
#else
    if(end_eventts() > MAXBUFF)
#endif
    {
      sca();
      end_block();
      chkblk = 0;
#ifndef DBUFF
      chkbuff = 1;
#else
      chmem();
      chkbuff++;
      if(chkbuff != 2){
	clear();
	vme_interrupt_enable2(INTLEVEL);
	vme_interrupt_clear2(INTLEVEL);
      }else{
      }
#endif
    }else{
      clear();
      vme_interrupt_enable2(INTLEVEL);
      vme_interrupt_clear2(INTLEVEL);
    }
  }else{
    printk("babildrv: interrupt not asserted !!\n");
    vme_interrupt_asserted2(INTLEVEL);
    vme_interrupt_clear2(INTLEVEL);
    printk("babildrv: after clear\n");
    return IRQ_NONE;
  }
  
  wake_up_interruptible(&babil_wait_queue);

  return IRQ_HANDLED;
}

#if LINUX_VERSION_CODE >= 0x020600
module_init(babildrv_init_module);
module_exit(babildrv_cleanup_module);
#endif
