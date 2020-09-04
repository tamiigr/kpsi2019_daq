/* Mesytec MQDC32 */

#include "mqdc32.h"

void mqdc32_map_clear(int n){
  short val;
  val=1;
  univ_map_write16(MQDC32_READOUT_RESET, &val, n);
}

void mqdc32_map_readout_clear(int n){
  mqdc32_map_clear(n);
}

void mqdc32_map_marking_type(short val, int n){
  univ_map_write16(MQDC32_MARKING_TYPE, &val, n);
  // 0:event counter, 1:time stamp, 0x11:extended time stamp
}

void mqdc32_map_start_acq(int n){
  short val;
  val=1;
  univ_map_write16(MQDC32_START_ACQ, &val, n);  
}

void mqdc32_map_stop_acq(int n){
  short val;
  val=0;
  univ_map_write16(MQDC32_START_ACQ, &val, n);  
}

void mqdc32_map_bank_operation(short val, int n){
  univ_map_write16(MQDC32_BANK_OPERATION, &val, n);  
}

void mqdc32_map_irq_level(short val, int n){
  univ_map_write16(MQDC32_IRQ_LEVEL, &val, n);  
}

void mqdc32_map_irq_threshold(short val, int n){
  univ_map_write16(MQDC32_IRQ_THRESHOLD, &val, n);  
}

void mqdc32_map_fifo_reset(int n){
  short val;
  val=1;
  univ_map_write16(MQDC32_FIFO_RESET, &val, n);  
}

void mqdc32_map_nim_gat1_osc(short val, int n){
  univ_map_write16(MQDC32_NIM_GAT1_OSC, &val, n);
  // 0:gate1 input, 1:time stamp input, also set0x6096
}

void mqdc32_map_nim_fc_reset(short val, int n){
  univ_map_write16(MQDC32_NIM_FC_RESET, &val, n);
  // 0:fast clear, 1:reset time stamp, 2:input for experimental trig
}

void mqdc32_map_nim_busy(short val, int n){
  univ_map_write16(MQDC32_NIM_BUSY, &val, n);
  // 0:busy, 3:Cbus out, 4:buffer full, 
  // 8:data above threshold, 9:event above threshold
}

void mqdc32_map_reset_ctr_ab(short val, int n){
  univ_map_write16(MQDC32_RESET_CTR_AB, &val, n);
  // b0001: reset all counters in CTRA
  // b0002: reset all counters in CTRB
}

void mqdc32_map_ts_sources(short val, int n){
  univ_map_write16(MQDC32_TS_SOURCES, &val, n);
  // bit0(VME=0, external=1)
  // bit1(external reset enable)
}

void mqdc32_map_module_id(short val, int n){
  univ_map_write16(MQDC32_MODULE_ID, &val, n);
}

void mqdc32_map_fast_vme(int n){
  short val;
  val=0xfa;
  univ_map_write16(MQDC32_FAST_VME, &val, n);  
}

void mqdc32_map_limit_bank_0(short val, int n){
  univ_map_write16(MQDC32_LIMIT_BANK_0, &val, n);
}

void mqdc32_map_limit_bank_1(short val, int n){
  univ_map_write16(MQDC32_LIMIT_BANK_1, &val, n);
}


int mqdc32_map_segdata(int n){
  volatile short cnt, i;
  univ_map_read16(MQDC32_BUFFER_DATA_LENGTH, (short *)&cnt, n);
  cnt = cnt & 0x3fff;
  for(i=0;i<cnt;i++){
    univ_map_read32(MQDC32_DATA, (long *)(data+mp), n);
    mp += 2;
    segmentsize += 2;
  }
  return segmentsize;
}

int mqdc32_map_dma_segdata(int ndma, int n){
  short dmacnt;
  univ_map_read16(MQDC32_BUFFER_DATA_LENGTH, (short *)(&dmacnt), n);
  dmacnt = (dmacnt & 0x3fff)*4;
  if(dmacnt > DMASIZE) dmacnt=DMASIZE;
  return univ_dma_segdata(dmacnt, ndma);
}
