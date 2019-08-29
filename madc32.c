/* madc32 */

#include "madc32.h"

void madc32_map_clear(int n){
  short val;
  val = 1;
  univ_map_write16(MADC32_READOUT_RESET, &val, n);
}

void madc32_map_readout_reset(int n){
  madc32_map_clear(n);
}

void madc32_map_start_acq(int n){
  short val;
  val = 1;
  univ_map_write16(MADC32_START_ACQ, &val, n);
}

void madc32_map_stop_acq(int n){
  short val;
  val = 0;
  univ_map_write16(MADC32_START_ACQ, &val, n);
}

void madc32_map_irq_level(short val, int n){
  univ_map_write16(MADC32_IRQ_LEVEL, &val, n);
}

void madc32_map_irq_threshold(short val, int n){
  univ_map_write16(MADC32_IRQ_THRESHOLD, &val, n);
}

void madc32_map_max_transfer_data(short val, int n){
  univ_map_write16(MADC32_MAX_TRANSFER_DATA, &val, n);
}

void madc32_map_marking_type(short val, int n){
  univ_map_write16(MADC32_MARKING_TYPE, &val, n);
}

void madc32_map_nim_gat1_osc(short val, int n){
  univ_map_write16(MADC32_NIM_GAT1_OSC, &val, n);
}

void madc32_map_ts_sources(short val, int n){
  univ_map_write16(MADC32_TS_SOURCES, &val, n);
}

void madc32_map_threshold(short ch, short val, int n){
  univ_map_write16(MADC32_THRESHOLD+ch*2, &val, n);
}

void madc32_map_fifo_reset(int n){
  short val;
  val=1;
  univ_map_write16(MADC32_FIFO_RESET, &val, n);  
}

void madc32_map_reset_ctr_ab(int n){
  short val;
  val=1;
  univ_map_write16(MADC32_RESET_CTR_AB, &val, n);    
}

void madc32_map_resol_2k(int n){
  short val;
  val=0;
  univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
}

void madc32_map_resol_4k(int n){
  short val;
  val=1;
  univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
}

void madc32_map_resol_4khires(int n){
  short val;
  val=2;
  univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
}

void madc32_map_resol_8khires(int n){
  short val;
  val = 4;
  univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
}

void madc32_map_module_id(short val, int n){
  univ_map_write16(MADC32_MODULE_ID, &val, n);
}

void madc32_map_input_range(short val, int n){
  univ_map_write16(MADC32_INPUT_RANGE, &val, n);  
}

void madc32_map_use_gg(short val, int n){
  univ_map_write16(MADC32_USE_GG, &val, n);  
}

void madc32_map_hold_delay(short ch, short val, int n){
  univ_map_write16(MADC32_HOLD_DELAY0+ch*2, &val, n);  
}

void madc32_map_hold_width(short ch, short val, int n){
  univ_map_write16(MADC32_HOLD_WIDTH0+ch*2, &val, n);    
}

void madc32_map_nim_busy(short val, int n){
  univ_map_write16(MADC32_NIM_BUSY, &val, n);      
}

void madc32_map_multi_event(short val, int n){
  univ_map_write16(MADC32_MULTIEVENT, &val, n);      
}

void madc32_map_bank_operation(short val, int n){
  univ_map_write16(MADC32_BANK_OPERATION, &val, n);
}

int madc32_map_segdata(int n){
  volatile short cnt, i;
  univ_map_read16(MADC32_BUFFER_DATA_LENGTH, (short *)&cnt, n);
  cnt = cnt & 0x3fff;
  for(i=0;i<cnt;i++){
    univ_map_read32(MADC32_DATA, (long *)(data+mp), n);
    mp += 2;
    segmentsize += 2;
  }
  return segmentsize;
}

int madc32_map_dma_segdata(int ndma, int n){
  short dmacnt;
  univ_map_read16(MADC32_BUFFER_DATA_LENGTH, (short *)(&dmacnt), n);
  dmacnt = (dmacnt & 0x3fff)*4;
  if(dmacnt > DMASIZE) dmacnt=DMASIZE;
  return univ_dma_segdata(dmacnt, ndma);
}
