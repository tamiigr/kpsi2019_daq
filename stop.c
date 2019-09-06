void unmap(void){
univ_end_window(LUPOMAPN);

#ifdef USE_1190
  univ_end_window(V1190_MAPN);
  //univ_end_dma(2);
  //univ_end_dma(3);
  //univ_end_dma(4);
#endif

#ifdef USE_MADC
  univ_end_window(MADC32_MAPN);
  univ_end_dma(1);
#endif

  univ_end_window(V977_MAPN);

#ifdef USE_V775
  univ_end_window(V775_MAPN);
#endif

    printk("unmap done\n");
}


void stop(void){
 // rpv130_map_pulse(OPSCASTOP, RPV130MAPN);

 // v977_map_int_mask(0xffff, V977_MAPN);
 // v977_map_int_level(0, V977_MAPN);
//  v977_map_out_clear(V977_MAPN);
//  rpv130_map_clear(RPV130MAPN);

#ifdef USE_MADC
 // madc32_map_stop_acq(MADC32_MAPN);
  madc32_map_clear(MADC32_MAPN);
  madc32_map_irq_level(0,MADC32_MAPN); //newly added
#endif


#ifdef USE_V775
  v7XX_map_int_level(0, V775_MAPN);
#endif
 // vlupodm_disable_interrupt_map(LUPOMAPN);
  vlupodm_clear_map(LUPOMAPN);

  printk("DAQ stop.\n");
  printk("\n");
}
