void map(void){
  univ_init_window(LUPOADDR, REGSIZE, A32, LUPOMAPN); 

  /* V1190 mapping */
#ifdef USE_1190
  univ_init_window(V1190_BASE_ADR, V1190_MAP_SIZE, A32, V1190_MAPN);
  univ_init_dma(V1190_BASE_ADR, DMASIZE, V1190_MAPN);
#endif

  /* MADC32 mapping */
#ifdef USE_MADC
  univ_init_window(MADC32_BASE_ADR, MADC32_MAP_SIZE, A32, MADC32_MAPN);
#ifdef DMA_MADC
  univ_init_dma(MADC32_BASE_ADR, DMASIZE, MADC32_MAPN);
#endif
#endif

  /* MQDC32 mapping */
#ifdef USE_MQDC
  univ_init_window(MQDC32_BASE_ADR1, MQDC32_MAP_SIZE, A32, MQDC32_MAPN1);
  univ_init_window(MQDC32_BASE_ADR2, MQDC32_MAP_SIZE, A32, MQDC32_MAPN2);
#ifdef DMA_MQDC
  univ_init_dma(MQDC32_BASE_ADR1, DMASIZE, MQDC32_MAPN1);
  univ_init_dma(MQDC32_BASE_ADR2, DMASIZE, MQDC32_MAPN2);
#endif
#endif

  /* V260 mapping */
#ifdef USE_V260
  univ_init_window(V260_ADR, V260_REGSIZE, A24, V260_MAPN);  
#endif

  printk("mapping done\n");
}

void startup(void){
  short val;

  /* Startup Function */
  vme_define_intlevel(INTLEVEL);

  // Output Config
  val = 0x1;
  vlupodm_outconf0_map(LUPOMAPN, val);
  //  val = 0x4;
  val = 0x1;
  vlupodm_outconf1_map(LUPOMAPN, val);
  val = 0x32;
  vlupodm_outconf2_map(LUPOMAPN, val);
  val = 0x10;
  vlupodm_outconf3_map(LUPOMAPN, val); 

  vlupodm_clear_map(LUPOMAPN);
  
  // vlupodm_enable_interrupt_map(LUPOMAPN);
  
 // initialize V1190
#ifdef USE_1190
  v1X90_multi_map_evt_reset(0, V1190_MAPN);
  v1X90_multi_map_intlevel(7, 0, V1190_MAPN);
  v1X90_multi_map_almost_full(0xffff, 0, V1190_MAPN);
  v1X90_multi_map_cnt_reg(0x128, 0, V1190_MAPN);
#endif

  /* configure MADC32 */
#ifdef USE_MADC
  madc32_map_stop_acq(MADC32_MAPN);
  madc32_map_module_id(0, MADC32_MAPN);
  madc32_map_irq_level(0, MADC32_MAPN);
  madc32_map_input_range(0x1, MADC32_MAPN);   // 0:4V, 1:10V, 2:8V
  madc32_map_use_gg(0x1, MADC32_MAPN);
  madc32_map_hold_delay(0, 0, MADC32_MAPN);
  madc32_map_hold_width(0, 40, MADC32_MAPN);  // in unit of 50ns, 80=4000ns
  madc32_map_nim_busy(1, MADC32_MAPN);        // internal gate output
  madc32_map_marking_type(1, MADC32_MAPN);
  madc32_map_ts_sources(1, MADC32_MAPN);
  madc32_map_nim_gat1_osc(1, MADC32_MAPN);
  madc32_map_start_acq(MADC32_MAPN);
  madc32_map_clear(MADC32_MAPN);
  madc32_map_fifo_reset(MADC32_MAPN);
  madc32_map_reset_ctr_ab(MADC32_MAPN);       // reset time stamp/trig num
  madc32_map_irq_level(INTLEVEL, MADC32_MAPN);
#endif

  /* configure MQDC32 */
#ifdef USE_MQDC
  /* NaI QDC */
  mqdc32_map_stop_acq(MQDC32_MAPN1);
  mqdc32_map_module_id(1, MQDC32_MAPN1);
  mqdc32_map_marking_type(0, MQDC32_MAPN1);
  mqdc32_map_bank_operation(0, MQDC32_MAPN1);
  mqdc32_map_limit_bank_1(255, MQDC32_MAPN1);  //255: no limitation
  mqdc32_map_nim_gat1_osc(0, MQDC32_MAPN1);
  //  mqdc32_map_fast_vme(MQDC32_MAPN1);
  mqdc32_map_start_acq(MQDC32_MAPN1);  
  mqdc32_map_clear(MQDC32_MAPN1);
  mqdc32_map_fifo_reset(MQDC32_MAPN1);
  mqdc32_map_reset_ctr_ab(1, MQDC32_MAPN1);
  mqdc32_map_irq_level(0, MQDC32_MAPN1);

  /* LaBr+plastic QDC */
  mqdc32_map_stop_acq(MQDC32_MAPN2);
  mqdc32_map_module_id(2, MQDC32_MAPN2);
  mqdc32_map_marking_type(0, MQDC32_MAPN2);
  mqdc32_map_bank_operation(0, MQDC32_MAPN2);
  mqdc32_map_limit_bank_1(255, MQDC32_MAPN2);  //255: no limitation
  mqdc32_map_nim_gat1_osc(0, MQDC32_MAPN2);
  //  mqdc32_map_fast_vme(MQDC32_MAPN2);
  mqdc32_map_start_acq(MQDC32_MAPN2);  
  mqdc32_map_clear(MQDC32_MAPN2);
  mqdc32_map_fifo_reset(MQDC32_MAPN2);
  mqdc32_map_reset_ctr_ab(1, MQDC32_MAPN2);
  mqdc32_map_irq_level(0, MQDC32_MAPN2);
#endif
  
  /* initialize v260 */
#ifdef USE_V260  
  v260_map_clear(V260_MAPN);
#endif

  madc32_map_start_acq(MADC32_MAPN);

  /* VETO clear, start DAQ */
  vlupodm_pulse_map(LUPOMAPN,1);

  printk("DAQ start.\n");
}
