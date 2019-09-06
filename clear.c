void clear(void){
    printk("clear\n");
  madc32_map_irq_level(INTLEVEL,MADC32_MAPN); //newly added
  vlupodm_clear_map(LUPOMAPN);
#ifdef USE_V775
  //  v7XX_map_clear(V775_MAPN);
  v7XX_map_int_level(INTLEVEL, V775_MAPN);
  v7XX_map_event_trigger_register(1, V775_MAPN);
#endif

#ifdef USE_1190
   v1X90_map_clear(V1190_MAPN);
  //v1X90_multi_map_clear(V1190_MAIKO_ADR - V1190_BASE_ADR,V1190_MAPN);
  //v1X90_multi_map_clear(V1190_BDC1_ADR  - V1190_BASE_ADR,V1190_MAPN);
  //v1X90_multi_map_clear(V1190_BDC2_ADR  - V1190_BASE_ADR,V1190_MAPN);
#endif

 
     vlupodm_pulse_map(LUPOMAPN,1);
}
