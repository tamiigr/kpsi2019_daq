/////////////////////////////////////////////////////////
// Driver for TMB2 memory board
//  This driver is writtern for DAQ upgrade with V7768
//    ver. 2017/07/27  T. Furuno
//     DMA readout confirmed to work
/////////////////////////////////////////////////////////

#include "tmb2.h"

void tmb2_multi_map_reset(unsigned long offset, int nmap){
  long sval;
  sval=TMB2_CONT_CRESET;
  univ_map_write32(offset+TMB2_CONTREG, &sval, nmap);
}

void tmb2_multi_map_switchbuf(unsigned long offset, int nmap){
  long sval;
  sval=TMB2_CONT_BUFCHANGE;
  univ_map_write32(offset+TMB2_CONTREG, &sval, nmap);
}

void tmb2_multi_map_start(unsigned long offset, int nmap){
  long sval;
  sval=TMB2_CONT_START;
  univ_map_write32(offset+TMB2_CONTREG, &sval, nmap);  
}

void tmb2_multi_map_stop(unsigned long offset, int nmap){
  long sval;
  sval=TMB2_CONT_STOP;
  univ_map_write32(offset+TMB2_CONTREG, &sval, nmap);  
}

void tmb2_multi_map_clear(unsigned long offset, int nmap){
  long zero_flag, sval;
  zero_flag=TMB2_CONT_CLALL0;
  sval=TMB2_CONT_CLEAR;
  univ_map_write32(offset+TMB2_CONTREG, &zero_flag, nmap);    
  univ_map_write32(offset+TMB2_CONTREG, &sval, nmap);    
}

void tmb2_multi_map_setdepth(unsigned long offset, int icn, long depth, int nmap){
  univ_map_write32(offset+TMB2_DEPREG1+icn*8, &depth, nmap);
}

int tmb2_multi_map_segdata(unsigned long offset, int icn, int depth, int nmap){
  long ilp;
  for(ilp=0; ilp<depth; ilp++){
    univ_map_read32(offset+TMB2_OUTBUFF1+icn*0x800, (long *)(data+mp), nmap);
    mp+=2;
    segmentsize+=2;
  }
  return segmentsize;
}

int tmb2_multi_map_readstat(unsigned long offset, int icn, int nmap){
  long istat;
  univ_map_read32(offset+TMB2_STAREG1+icn*8, &istat, nmap);
  return istat;
}

int tmb2_multi_map_readdepth(unsigned long offset, int icn, int nmap){
  long depth;
  univ_map_read32(offset+TMB2_DEPREG1+icn*8, &depth, nmap);
  return depth;
}

int tmb2_multi_map_readcnt(unsigned long offset, int icn, int nmap){
  long cnt;
  univ_map_read32(offset+TMB2_COUNTREG1+icn*8, &cnt, nmap);
  return cnt;
}

int tmb2_multi_map_verreg(unsigned long offset, int nmap){
  long ver;
  univ_map_read32(offset+TMB2_VERREG, &ver, nmap);
  return ver;
}


int tmb2_multi_map_dmasegdata(unsigned long ndma, int offset, 
			      int icn, long depth, int nmap){
  int dmacnt;
  dmacnt = depth*4;
  if(dmacnt>DMASIZE){
    dmacnt=DMASIZE;
    printk("Too many data in TMB2!! mem:%d port:%d\n", offset, icn);
  }
  //  return univ_dma_segdata(dmacnt, ndma); // data structure is bad
  return univ_dma_segdata_tmb(dmacnt, ndma); // changed on 17/09/03
}
