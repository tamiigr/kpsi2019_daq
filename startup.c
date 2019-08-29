void map(void){
  /*RPV130 mapping*/
  univ_init_window(RPV130ADR, RPV130_MAP_SIZE, A16, RPV130MAPN);
  
  /* V977 mapping */
  univ_init_window(V977ADR, V977REGSIZE, A32, V977_MAPN);

  /* V1190 mapping */
  univ_init_window(V1190_BASE_ADR, V1190_MAP_SIZE, A32, V1190_MAPN);
  univ_init_dma(V1190_MAIKO_ADR, DMASIZE, 2);
  univ_init_dma(V1190_BDC1_ADR,  DMASIZE, 3);
  univ_init_dma(V1190_BDC2_ADR,  DMASIZE, 4);

  /* MADC32 mapping */
  univ_init_window(MADC32_BASE_ADR, MADC32_MAP_SIZE, A32, MADC32_MAPN);
  univ_init_dma(V1190_MAIKO_ADR, DMASIZE, 1);  

  /* TMB2 mapping*/
  univ_init_window(TMB2_BASEADR, TMB2_REGSIZE, A32, TMB2_MAPN);  
  univ_init_dma(TMB2_ADR0,       DMASIZE, 5);
  univ_init_dma(TMB2_ADR1,       DMASIZE, 6);
  univ_init_dma(TMB2_ADR2,       DMASIZE, 7);
  univ_init_dma(TMB2_ADR2+0x800, DMASIZE, 8);

  /* V830 mapping */
  univ_init_window(V830_ADR, V830_REGSIZE, A32, V830_MAPN);  

  /* V775 mapping */
#ifdef USE_V775
  univ_init_window(V775_ADR, V775_REGSIZE, A32, V775_MAPN);  
#endif

  printk("mapping done\n");
}

void startup(void){
  unsigned long TMB2_OFF[3]={TMB2_ADR0-TMB2_BASEADR, TMB2_ADR1-TMB2_BASEADR, TMB2_ADR2-TMB2_BASEADR};
  int imem, icn;

  /* Startup Function */
  vme_define_intlevel(INTLEVEL);

  /* initialize RPV130 */
#ifdef USE_RPV
  //vme_read_intvector();
  rpv130_map_clear(RPV130MAPN);
  rpv130_map_level(0, RPV130MAPN);
#endif

  /* initialize v1190s */
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
    
  /* initialize MADC32 */
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
  
  /* set thresholds */
  short ich;
  for(ich=0; ich<32; ich++){
    madc32_map_threshold(ich, 0x1fff, MADC32_MAPN); // switch off
  }
  madc32_map_threshold(5, 0, MADC32_MAPN);
  madc32_map_threshold(6, 0, MADC32_MAPN);
  madc32_map_threshold(7, 0, MADC32_MAPN);
  madc32_map_threshold(8, 0, MADC32_MAPN);
#endif

  /* initialize v977 */
  v977_map_out_clear(V977_MAPN);
  v977_map_int_mask(0, V977_MAPN);
  v977_map_int_level(INTLEVEL, V977_MAPN);

  /* initialize v775 */
#ifdef USE_V775
  v7XX_map_clear(V775_MAPN);
  //  v7XX_map_set_interrupt(INTLEVEL, 0, V775_MAPN);
  v7XX_map_int_level(INTLEVEL, V775_MAPN);
  v7XX_map_event_trigger_register(1, V775_MAPN);
  v775_map_conf1(0x5800, V775_MAPN);
  v775_map_conf2(~0x5800, V775_MAPN);
  v775_map_full_scale_range(0x1e, V775_MAPN);
  v775_map_set_cnt_reg(0x20, V775_MAPN);
#endif

  /* initialize v830 */
  v830_map_soft_clear(V830_MAPN);

  /* initialize TMB2 */
#ifdef USE_TMB2
  tmb2_multi_map_stop(TMB2_OFF[0], TMB2_MAPN);
  tmb2_multi_map_stop(TMB2_OFF[1], TMB2_MAPN);
  tmb2_multi_map_stop(TMB2_OFF[2], TMB2_MAPN);
  
  for(imem=0; imem<TMB2_NMEM; imem++){
    for(icn=0; icn<TMB2_NCN; icn++){
      tmb2_multi_map_setdepth(TMB2_OFF[imem], icn, TMB2DEPTH, TMB2_MAPN);
    }
    tmb2_multi_map_switchbuf(TMB2_OFF[imem], TMB2_MAPN);
    tmb2_multi_map_clear(TMB2_OFF[imem], TMB2_MAPN);
  }
#endif

  /* start MADC32 */
#ifdef USE_MADC
  madc32_map_fifo_reset(MADC32_MAPN);
  madc32_map_reset_ctr_ab(MADC32_MAPN);  // reset time stamp
  madc32_map_start_acq(MADC32_MAPN);
  madc32_map_readout_reset(MADC32_MAPN); // allow new trigger
#endif
  
  /* Start TMB2 */
#ifdef USE_TMB2
  for(imem=0; imem<TMB2_NMEM; imem++){
    int delayloop;
    tmb2_multi_map_start(TMB2_OFF[imem], TMB2_MAPN);
    for(delayloop=0; delayloop<1000000; delayloop++){
      if(tmb2_multi_map_readstat(TMB2_OFF[imem],0,TMB2_MAPN)&TMB2_STAT_RUN){
	break;
      }
      delay_us();
      tmb2_multi_map_start(TMB2_OFF[imem], TMB2_MAPN);
    }
  }
#endif

  //Start DAQ
#ifdef USE_RPV
  rpv130_map_pulse(OPIWKRST, RPV130MAPN);
  rpv130_map_pulse(OPSCACLEAR, RPV130MAPN);
  rpv130_map_pulse(OPBUSYCL|OPSCASTART,RPV130MAPN);
#endif

  printk("DAQ start.\n");
}
