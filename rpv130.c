#include "rpv130.h"

int rpv130_write( unsigned long maddr, unsigned short val){
  set_amsr(0x29);
  vwrite16(maddr, &val);
  set_amsr(0x09);
  return 1;
}

int rpv130_map_write(unsigned long off, unsigned short val, int n){
  univ_map_write16(off, &val, n);
  return 1;
}

// added by inaba 
int rpv130_map_pulse(unsigned short val, int n){
  univ_map_write16(RPV130_PULSE, &val, n);

  return 1;
}

int rpv130_map_level(unsigned short val, int n){
  univ_map_write16(RPV130_LEVEL, &val, n);

  return 1;
}


int rpv130_level(unsigned long maddr, unsigned short val){
  rpv130_write(maddr+RPV130_LEVEL, val);

  return 1;
}


int rpv130_segdata(unsigned long maddr, int mode){
  set_amsr(0x29);
  vread16(maddr+mode,(short *)(data+mp));
  set_amsr(0x09);
  mp += 1;
  segmentsize += 1;
  
  return segmentsize;
}

int rpv130_segdata_v(unsigned long maddr, int mode, unsigned short *sval){
  set_amsr(0x29);
  vread16(maddr+mode,(short *)(data+mp));
  set_amsr(0x09);

  memcpy((char *)sval,(char *)(data+mp),2);

  mp += 1;
  segmentsize += 1;
  
  return segmentsize;
}

int rpv130_clear(unsigned long maddr){
  short sval;

  sval = RPV130_CLEAR1OR2 | RPV130_CLEAR3 | RPV130_MASK1OR2;
  //sval = RPV130_CLEAR1OR2 | RPV130_CLEAR3;
  set_amsr(0x29);
  vwrite16(maddr+RPV130_CTL1,&sval);
  vwrite16(maddr+RPV130_CTL2,&sval);
  set_amsr(0x09);

  return 1;
}

int rpv130_map_clear(int n){
  short sval;

  //  sval = RPV130_CLEAR1OR2 | RPV130_CLEAR3 | RPV130_MASK1OR2;
  
  /* changed on 17/08/30 */
  sval = RPV130_CLEAR1OR2 | RPV130_CLEAR3;
//  set_amsr(0x29);
  univ_map_write16(RPV130_CTL1,&sval,n);
  univ_map_write16(RPV130_CTL2,&sval,n);
//  set_amsr(0x09);

  return 1;
}

int rpv130_inton(unsigned long maddr){
  short sval;

  sval = RPV130_ENABLE1OR2 | RPV130_MASK1OR2;
  //sval = RPV130_ENABLE1OR2;
  set_amsr(0x29);
  vwrite16(maddr+RPV130_CTL1,&sval);
  vwrite16(maddr+RPV130_CTL2,&sval);
  set_amsr(0x09);

  return 1;
}

int rpv130_map_inton(int n){
  short sval;
 
  sval = RPV130_ENABLE1OR2 | RPV130_MASK1OR2;
//  set_amsr(0x29);
  univ_map_write16(RPV130_CTL1,&sval,n);
  univ_map_write16(RPV130_CTL2,&sval,n);
//  set_amsr(0x09);

  return 1;
}
