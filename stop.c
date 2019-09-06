void unmap(void){
  univ_end_window(LUPOMAPN);

#ifdef USE_1190
  univ_end_window(V1190_MAPN);
  univ_end_dma(2);
#endif

#ifdef USE_MADC
  univ_end_window(MADC32_MAPN);
  //  univ_end_dma(1);
#endif


#ifdef USE_MQDC
  univ_end_window(MQDC32_MAPN);
  //  univ_end_dma(1);
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
  mqdc32_map_stop_acq(MQDC32_MAPN);
  mqdc32_map_clear(MQDC32_MAPN);
#endif

 // vlupodm_disable_interrupt_map(LUPOMAPN);
  vlupodm_clear_map(LUPOMAPN);

  printk("DAQ stop.\n");
  printk("\n");
}
