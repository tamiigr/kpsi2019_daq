void clear(void){
  //  printk("clear\n");
  //  v977_map_outset(0, V977_MAPN);
  v977_map_out_clear(V977_MAPN);
  v977_map_int_mask(0, V977_MAPN);
  v977_map_int_level(INTLEVEL, V977_MAPN);
  
#ifdef USE_V775
  //  v7XX_map_clear(V775_MAPN);
  v7XX_map_int_level(INTLEVEL, V775_MAPN);
  v7XX_map_event_trigger_register(1, V775_MAPN);
#endif

#ifdef USE_1190
  v1X90_multi_map_clear(V1190_MAIKO_ADR - V1190_BASE_ADR,V1190_MAPN);
  v1X90_multi_map_clear(V1190_BDC1_ADR  - V1190_BASE_ADR,V1190_MAPN);
  v1X90_multi_map_clear(V1190_BDC2_ADR  - V1190_BASE_ADR,V1190_MAPN);
#endif

  rpv130_map_level(0, RPV130MAPN);
  rpv130_map_clear(RPV130MAPN);

  if(mpflag){
    rpv130_map_pulse(OPBUSYCL,RPV130MAPN);
  }
  //  rpv130_map_pulse(OPBUSYCL,RPV130MAPN);
}
