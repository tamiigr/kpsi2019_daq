void unmap(void){
  univ_end_window(LUPOMAPN);

#ifdef USE_MADC
  univ_end_window(MADC32_MAPN);
#ifdef DMA_MADC  
  univ_end_dma(MADC32_MAPN);
#endif
#endif


#ifdef USE_MQDC
  univ_end_window(MQDC32_MAPN1);
  univ_end_window(MQDC32_MAPN2);
#ifdef DMA_MQDC  
  univ_end_dma(MQDC32_MAPN1);
  univ_end_dma(MQDC32_MAPN2);
#endif
#endif

#ifdef USE_1190
  univ_end_window(V1190_MAPN);
  univ_end_dma(V1190_MAPN);
#endif

  printk("unmap done\n");
}


void stop(void){

#ifdef USE_MADC
  madc32_map_stop_acq(MADC32_MAPN);
  madc32_map_clear(MADC32_MAPN);
  madc32_map_irq_level(0,MADC32_MAPN); //newly added
#endif

#ifdef USE_MQDC
  mqdc32_map_stop_acq(MQDC32_MAPN1);
  mqdc32_map_clear(MQDC32_MAPN1);
  mqdc32_map_stop_acq(MQDC32_MAPN2);
  mqdc32_map_clear(MQDC32_MAPN2);
#endif

 // vlupodm_disable_interrupt_map(LUPOMAPN);
  vlupodm_clear_map(LUPOMAPN);

  printk("DAQ stop.\n");
  printk("\n");
}
