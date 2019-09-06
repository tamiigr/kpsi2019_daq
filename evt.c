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
     printk("Event \n");
  //rpv130_map_pulse(0x2,RPV130MAPN);
 madc32_map_irq_level(0,MADC32_MAPN); //newly added
  /* TMB2 parameters */

  unsigned short irpv;
 

  /* Reset IRQ */
 // vme_read_intvector();
 // v977_map_int_mask(0xffff, V977_MAPN);
 // v977_map_int_level(0, V977_MAPN);
 // v977_map_outset(0, V977_MAPN);
 // v977_map_out_clear(V977_MAPN);

#ifdef USE_V775
  v7XX_map_int_level(0, V775_MAPN);
#endif

// again:
//  vme_read_intvector();
//  irpv=0;
//  rpv130_map_clear(RPV130MAPN);



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
  
   madc32_map_clear(MADC32_MAPN);
  //madc32_map_fifo_reset(MADC32_MAPN);
  //madc32_map_readout_reset(MADC32_MAPN);  // allow new trigger
#endif

/*
  // busy clear 
  if(mp<MAXBUFF){
    rpv130_map_pulse(OPBUSYCL,RPV130MAPN);
    mpflag=0;
  }
  else{
    mpflag=1;
  }
*/

 
/*
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
*/
    madc32_map_irq_level(0,MADC32_MAPN); //newly added
}
