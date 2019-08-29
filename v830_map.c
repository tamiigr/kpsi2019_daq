#include "v830_map.h"
/* 
 * original by Masaru-san 
 * reviced by S.Ota 6.Nov.2003.
 */
void v830_initialize(unsigned long maddr) {
  long dummy = 0x1;
  vwrite32(maddr+V830_GLOBAL_COUNT_ENABLE, &dummy);
}

void v830_initialize_map(int n) {
  long dummy = 0x1;
  univ_map_write32(V830_GLOBAL_COUNT_ENABLE, &dummy, n);
}

int v830_clear_all(unsigned long maddr){
  long dummy = 0x1;
  vwrite32(maddr+V830_CLEAR_ALL_COUNTERS, &dummy);
  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  return 0;
}

int v830_clear_all_map(int n){
  long dummy = 0x1;
  univ_map_write32(V830_CLEAR_ALL_COUNTERS, &dummy, n);
  univ_map_write32(V830_CLOCK_SHADOW_REGISTER, &dummy, n);
  return 0;
}

int v830_read_scaler(short pos, short len, unsigned long maddr, unsigned short offset){
  volatile int i;
  // 
  // len must be 12*i
  long dummy = 0x1;
  //  mp = blksize/2 - (len * 2 * pos);           /* Set mp */
  // implicit declaration of blksize
#ifndef BLKSIZE
#define BLKSIZE 0x4000
#endif
  mp = BLKSIZE/2 - (len * 2 * pos);           /* Set mp */
  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  vwrite32(maddr+V830_CLEAR_ALL_COUNTERS, &dummy);
  for(i=0;i<len;i++){
    /* Read Scaler data */
    if(i<32){
      vread32(maddr+V830_READ_SHADOW_REGISTER+(offset+i)*4,(long *)(data+mp));
    }
    mp += 2;
  }

  return len * 2;
}

int v830_segdata(unsigned long maddr, int ch){
  long dummy = 0x1;

  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  vwrite32(maddr+V830_CLEAR_ALL_COUNTERS, &dummy);
  vread32(maddr+V830_READ_SHADOW_REGISTER+ch*4,(long *)(data+mp));
  mp += 2;
  segmentsize += 2;

  return segmentsize;
}

int v830_segndata(unsigned long maddr, int len){
  long dummy = 0x1;
  int i;
  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  vwrite32(maddr+V830_CLEAR_ALL_COUNTERS, &dummy);
  for(i=0;i<len;i++){
    if(i < 32){
      vread32(maddr+V830_READ_SHADOW_REGISTER+i*4,(long *)(data+mp));
      mp += 2;
      segmentsize += 2;
    }
  }

  return segmentsize;
}

void v830_setinhibit(unsigned long maddr){
  long dummy = V830_SET_INPUT_BIT0;

  vwrite32(maddr+V830_CONTROL_REGISTER, &dummy);

}

void v830_setinhibit_map(int n){
  long dummy = V830_SET_INPUT_BIT0;

  univ_map_write32(V830_CONTROL_REGISTER, &dummy, n);

}

int v830_segdatanc(unsigned long maddr, int ch){
  long dummy = 0x1;

  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  vread32(maddr+V830_READ_SHADOW_REGISTER+ch*4,(long *)(data+mp));
  mp += 2;
  segmentsize += 2;

  return segmentsize;
}


int v830_ridf_ncscaler_map(int n){
  int i;
//  long dummy = 0x1;

//  univ_map_write32(V830_CLOCK_SHADOW_REGISTER, &dummy, n);
  for(i=0;i<;i++){
    /* Read Scaler data */
    univ_map_read32(V830_READ_SHADOW_REGISTER+i*4,(long *)(data+mp), n);
    mp += 2;
  }

  scrsize += 2 * V830_NCH;
  if(eventsize > 0) eventsize += 2* V830_NCH;

  return scrsize;
}



int v830_ridf_scaler(unsigned long maddr){
  volatile int i;
  long dummy = 0x1;

  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  vwrite32(maddr+V830_CLEAR_ALL_COUNTERS, &dummy);
  for(i=0;i<V830_NCH;i++){
    /* Read Scaler data */
    vread32(maddr+V830_READ_SHADOW_REGISTER+i*4,(long *)(data+mp));
    mp += 2;
  }

  scrsize += 2 * V830_NCH;
  if(eventsize > 0) eventsize += 2* V830_NCH;

  return scrsize;
}

int v830_ridf_ncscaler(unsigned long maddr){
  volatile int i;
  long dummy = 0x1;

  vwrite32(maddr+V830_CLOCK_SHADOW_REGISTER, &dummy);
  for(i=0;i<V830_NCH;i++){
    /* Read Scaler data */
    vread32(maddr+V830_READ_SHADOW_REGISTER+i*4,(long *)(data+mp));
    mp += 2;
  }

  scrsize += 2 * V830_NCH;
  if(eventsize > 0) eventsize += 2* V830_NCH;

  return scrsize;
}

