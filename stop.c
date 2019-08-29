void unmap(void){

#ifdef USE_1190
  univ_end_window(V1190_MAPN);
  univ_end_dma(2);
  univ_end_dma(3);
  univ_end_dma(4);
#endif

#ifdef USE_MADC
  univ_end_window(MADC32_MAPN);
  univ_end_dma(1);
#endif

#ifdef USE_RPV
  univ_end_window(RPV130MAPN);
#endif

  univ_end_window(V977_MAPN);

#ifdef USE_V775
  univ_end_window(V775_MAPN);
#endif

#ifdef USE_TMB2
  univ_end_window(TMB2_MAPN);
  univ_end_dma(5);
  univ_end_dma(6);
  univ_end_dma(7);
  univ_end_dma(8);
#endif

    printk("unmap done\n");
}


void stop(void){
  rpv130_map_pulse(OPSCASTOP, RPV130MAPN);

  v977_map_int_mask(0xffff, V977_MAPN);
  v977_map_int_level(0, V977_MAPN);
  v977_map_out_clear(V977_MAPN);
  rpv130_map_clear(RPV130MAPN);

#ifdef USE_MADC
  madc32_map_stop_acq(MADC32_MAPN);
#endif

#ifdef USE_TMB2
  tmb2_multi_map_stop(TMB2_ADR0-TMB2_BASEADR, TMB2_MAPN);
  tmb2_multi_map_stop(TMB2_ADR1-TMB2_BASEADR, TMB2_MAPN);
  tmb2_multi_map_stop(TMB2_ADR2-TMB2_BASEADR, TMB2_MAPN);
#endif

#ifdef USE_V775
  v7XX_map_int_level(0, V775_MAPN);
#endif

  printk("DAQ stop.\n");
  printk("\n");
}
