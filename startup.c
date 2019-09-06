void map(void){
  univ_init_window(LUPOADDR, REGSIZE, A32, LUPOMAPN); 
  /* V977 mapping */
//  univ_init_window(V977ADR, V977REGSIZE, A32, V977_MAPN);

  /* V1190 mapping */
 // univ_init_window(V1190_BASE_ADR, V1190_MAP_SIZE, A32, V1190_MAPN);

  /* MADC32 mapping */
  univ_init_window(MADC32_BASE_ADR, MADC32_MAP_SIZE, A32, MADC32_MAPN);
 // univ_init_dma(V1190_MAIKO_ADR, DMASIZE, 1);  
  /* SIS3800*/
 // univ_init_window(SIS_BASE_ADR, SIS_MAP_SIZE, A32, SIS_MAPN);  

  /* V830 mapping */
  //univ_init_window(V830_ADR, V830_REGSIZE, A32, V830_MAPN);  

  /* V775 mapping */
//  univ_init_window(V775_ADR, V775_REGSIZE, A32, V775_MAPN);  

  printk("mapping done\n");
}

void startup(void){
  unsigned long TMB2_OFF[3]={TMB2_ADR0-TMB2_BASEADR, TMB2_ADR1-TMB2_BASEADR, TMB2_ADR2-TMB2_BASEADR};
  int imem, icn;
  short val;

  /* Startup Function */
  vme_define_intlevel(INTLEVEL);
/*
  // initialize MADC32, in MAIKO
#ifdef USE_MADC
  madc32_map_stop_acq(MADC32_MAPN);
  madc32_map_module_id(0, MADC32_MAPN);
  madc32_map_irq_level(0, MADC32_MAPN);
  madc32_map_resol_4k(MADC32_MAPN);
  madc32_map_input_range(0x1, MADC32_MAPN);   // 0:4V, 1:10V, 2:8V
  madc32_map_bank_operation(0, MADC32_MAPN);
  madc32_map_use_gg(0x1, MADC32_MAPN);
  madc32_map_hold_delay(0, 0, MADC32_MAPN);   // multiple of 50ns
  madc32_map_hold_width(0, 160, MADC32_MAPN); // multiple of 50ns
  madc32_map_nim_busy(1, MADC32_MAPN);        // 0:busy, 1:gate0
  madc32_map_multi_event(0, MADC32_MAPN);
  madc32_map_marking_type(1, MADC32_MAPN);    // 0:event num, 1:time stamp
  madc32_map_ts_sources(0, MADC32_MAPN);      // 0:VME, 1:external
  
  // set thresholds 
  short ich;
  for(ich=0; ich<32; ich++){
    madc32_map_threshold(ich, 0x1fff, MADC32_MAPN); // switch off
  }
  madc32_map_threshold(5, 0, MADC32_MAPN);
  madc32_map_threshold(6, 0, MADC32_MAPN);
  madc32_map_threshold(7, 0, MADC32_MAPN);
  madc32_map_threshold(8, 0, MADC32_MAPN);

#endif
*/


          // Output Config
             val = 0x1;
              vlupodm_outconf0_map(LUPOMAPN, val);
             val = 0x4;
              vlupodm_outconf1_map(LUPOMAPN, val);
              val = 0x32;
              vlupodm_outconf2_map(LUPOMAPN, val);
               val = 0x10;
               vlupodm_outconf3_map(LUPOMAPN, val); 
               vlupodm_clear_map(LUPOMAPN);

             // vlupodm_enable_interrupt_map(LUPOMAPN);



/*
 // initialize v1190s 
#ifdef USE_1190
  v1X90_multi_map_evt_reset(V1190_MAIKO_ADR - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_evt_reset(V1190_BDC1_ADR  - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_evt_reset(V1190_BDC2_ADR  - V1190_BASE_ADR, V1190_MAPN);

  v1X90_multi_map_intlevel(0x7, V1190_MAIKO_ADR - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_intlevel(0x7, V1190_BDC1_ADR  - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_intlevel(0x7, V1190_BDC2_ADR  - V1190_BASE_ADR, V1190_MAPN);

  v1X90_multi_map_almost_full(0xffff, V1190_MAIKO_ADR - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_almost_full(0xffff, V1190_BDC1_ADR  - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_almost_full(0xffff, V1190_BDC2_ADR  - V1190_BASE_ADR, V1190_MAPN);

  v1X90_multi_map_cnt_reg(0x128, V1190_MAIKO_ADR - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_cnt_reg(0x128, V1190_BDC1_ADR  - V1190_BASE_ADR, V1190_MAPN);
  v1X90_multi_map_cnt_reg(0x128, V1190_BDC2_ADR  - V1190_BASE_ADR, V1190_MAPN);
#endif
*/  

  // initialize v977 
 // v977_map_out_clear(V977_MAPN);
 // v977_map_int_mask(0, V977_MAPN);
 // v977_map_int_level(INTLEVEL, V977_MAPN);

 
  /* initialize v830 */
 // v830_map_soft_clear(V830_MAPN);

/*
  // start MADC32 
#ifdef USE_MADC
  madc32_map_fifo_reset(MADC32_MAPN);
  madc32_map_reset_ctr_ab(MADC32_MAPN);  // reset time stamp
  madc32_map_start_acq(MADC32_MAPN);
  madc32_map_readout_reset(MADC32_MAPN); // allow new trigger
#endif 
*/
  //Start DAQ

   // initialize v1190, KPSI2019
   //v1X90_map_clear(V1190_MAPN);
 

  //trigger from MADC
  madc32_map_stop_acq(MADC32_MAPN);
  madc32_map_module_id(0, MADC32_MAPN);
  madc32_map_irq_level(0, MADC32_MAPN);
   madc32_map_input_range(0x1, MADC32_MAPN);   // 0:4V, 1:10V, 2:8V
  madc32_map_start_acq(MADC32_MAPN);
  madc32_map_clear(MADC32_MAPN);
  madc32_map_irq_level(INTLEVEL, MADC32_MAPN);
  vlupodm_pulse_map(LUPOMAPN,1);
  printk("DAQ start.\n");
}
