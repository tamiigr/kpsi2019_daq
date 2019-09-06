void clear(void){

  madc32_map_irq_level(INTLEVEL,MADC32_MAPN); //newly added
  vlupodm_clear_map(LUPOMAPN);

#ifdef USE_1190
   v1X90_map_clear(V1190_MAPN);
#endif

   /* busy clear */
   vlupodm_pulse_map(LUPOMAPN,1);

}
