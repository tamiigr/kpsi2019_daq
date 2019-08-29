#include "rpv130.h"
#include "babirldrvcaen.h"
#include "tmb2.h"

#define _DEBUG_EVT 0
#define _DMA_MADC32 0
#define _DMA_TMB2 0

#define FADC_DATA 0x2
#define TPC_DATA 0x3
#define FOOTER_DATA 0xffffffff

void evt(void){
  rpv130_map_pulse(0x2,RPV130MAPN);
  /* TMB2 parameters */
#ifdef USE_TMB2
  int icn,imem;
  long depth[TMB2_NMEM][TMB2_NCN];
  unsigned long TMB2_OFF[3]={TMB2_ADR0-TMB2_BASEADR, 
			     TMB2_ADR1-TMB2_BASEADR, TMB2_ADR2-TMB2_BASEADR};
  int TMB2_DMAN[2][2]={{5,6},{7,8}};
#endif
  unsigned short irpv;
  //  printk("Event \n");

  /* Reset IRQ */
  vme_read_intvector();
  v977_map_int_mask(0xffff, V977_MAPN);
  v977_map_int_level(0, V977_MAPN);
  v977_map_outset(0, V977_MAPN);
  v977_map_out_clear(V977_MAPN);

#ifdef USE_V775
  v7XX_map_int_level(0, V775_MAPN);
#endif

 again:
  vme_read_intvector();
  irpv=0;
  rpv130_map_clear(RPV130MAPN);

#ifdef USE_TMB2
  /* Stop TMB2 */
  for(imem=0; imem<TMB2_NMEM; imem++){
    tmb2_multi_map_stop(TMB2_OFF[imem], TMB2_MAPN);
  }

  /* chech if the memorry is stopped */
  for(imem=0; imem<TMB2_NMEM; imem++){
    while(1){
      if(!(tmb2_multi_map_readstat(TMB2_OFF[imem],0,TMB2_MAPN)&TMB2_STAT_RUN)) break;
      delay_us();
      tmb2_multi_map_stop(TMB2_OFF[imem], TMB2_MAPN);      
    }
  }

  for(imem=0; imem<TMB2_NMEM; imem++){
    for(icn=0; icn<TMB2_NCN; icn++){
      /* Read memory counter */
      depth[imem][icn]=tmb2_multi_map_readcnt(TMB2_OFF[imem], icn, TMB2_MAPN);
      //      printk("imem=%d, icn=%d, cnt=%ld\n", imem,icn, depth[imem][icn]);
    }

    /* Switch buffer */
    tmb2_multi_map_switchbuf(TMB2_OFF[imem], TMB2_MAPN);

    /* Reset counter */
    tmb2_multi_map_reset(TMB2_OFF[imem], TMB2_MAPN);
  }

  /* Start memory */
  for(imem=0; imem<TMB2_NMEM; imem++){
    tmb2_multi_map_start(TMB2_OFF[imem], TMB2_MAPN);
  }

  /* Chech if the memory is ready */
  for(imem=0; imem<TMB2_NMEM; imem++){
    for(icn=0; icn<TMB2_NCN; icn++){
      while(1){
	if(tmb2_multi_map_readstat(TMB2_OFF[imem],icn,TMB2_MAPN)&TMB2_STAT_RUN) break;	
	delay_us();
	tmb2_multi_map_start(TMB2_OFF[imem], TMB2_MAPN);
      }
    }
  }
#endif // end of USE_TMB2

  /* Event */
  init_event();

  /* readout of v775 */
#ifdef USE_V775
  init_segment(SEGIDGEN(RCNPEN, F3, SSDT, V775));
  v7XX_map_segdata(V775_MAPN);
  end_segment();
  v7XX_map_clear(V775_MAPN);
#endif

  /*readout of v1190 */
#ifdef USE_1190
  init_segment(MKSEGID(RCNPEN,F3,SSDT,V1190));
  v1X90_multi_map_dma_segdata(2, V1190_MAIKO_ADR - V1190_BASE_ADR, V1190_MAPN); 
  v1X90_multi_map_dma_segdata(3, V1190_BDC1_ADR  - V1190_BASE_ADR, V1190_MAPN); 
  v1X90_multi_map_dma_segdata(4, V1190_BDC2_ADR  - V1190_BASE_ADR, V1190_MAPN); 
  end_segment(); 
#endif

  /* readout of MADC32 */
#ifdef USE_MADC
  init_segment(MKSEGID(RCNPEN,F3,SSDE,MADC32));
  madc32_map_segdata(MADC32_MAPN);
  //  madc32_map_dma_segdata(1, MADC32_MAPN); // DMA not work
  end_segment();
  
  madc32_map_fifo_reset(MADC32_MAPN);
  madc32_map_readout_reset(MADC32_MAPN);  // allow new trigger
#endif

  /* busy clear */
  if(mp<MAXBUFF){
    rpv130_map_pulse(OPBUSYCL,RPV130MAPN);
    mpflag=0;
  }
  else{
    mpflag=1;
  }


  /* readout of TMB2 */
#ifdef USE_TMB2
  // anode 0
  init_segment(MKSEGID(RCNPEN,F3,44,TMB2)); 
  tmb2_multi_map_dmasegdata(TMB2_DMAN[0][0], TMB2_OFF[0],0,
			    depth[0][0], TMB2_MAPN);
  end_segment();

  // anode 1
  init_segment(MKSEGID(RCNPEN,F3,45,TMB2)); 
  tmb2_multi_map_dmasegdata(TMB2_DMAN[0][1], TMB2_OFF[1],0,
			    depth[1][0], TMB2_MAPN);
  end_segment();

  // cathode 0
  init_segment(MKSEGID(RCNPEN,F3,46,TMB2)); 
  tmb2_multi_map_dmasegdata(TMB2_DMAN[1][0], TMB2_OFF[2],0,
			    depth[2][0], TMB2_MAPN);
  end_segment();

  // cathode 1
  init_segment(MKSEGID(RCNPEN,F3,47,TMB2)); 
  tmb2_multi_map_dmasegdata(TMB2_DMAN[1][1], TMB2_OFF[2],1,
			    depth[2][1], TMB2_MAPN);
  end_segment();

#endif

 if(!mpflag){
    univ_map_read16(RPV130_RSFF, &irpv, RPV130MAPN);
  }

  if(irpv & 0x1){
    end_event();
    //    printk("Event occurred during readout of TMB2.\n");
    int i;
    for(i=0; i<150; i++){
      delay_us();
    }
    //
    goto again;
  }

  if(!mpflag)rpv130_map_level(OPVETO, RPV130MAPN);
}
