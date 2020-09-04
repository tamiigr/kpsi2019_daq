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
  //  printk("Event \n");


  madc32_map_irq_level(0,MADC32_MAPN); //newly added
  
  /* Event */
  init_event();


  /* readout of MADC32 */
#ifdef USE_MADC
  init_segment(MKSEGID(RCNPEN,F3,SSDE,MADC32));

#ifndef DMA_MADC
  madc32_map_segdata(MADC32_MAPN);
#endif

#ifdef DMA_MADC
  madc32_map_dma_segdata(MADC32_MAPN, MADC32_MAPN);
#endif

  end_segment();
  madc32_map_clear(MADC32_MAPN);
#endif

  /* readout of MQDC32 */
#ifdef USE_MQDC

  /* readout for NaI */
  init_segment(MKSEGID(RCNPEN,F3,NAIE,MQDC32));
#ifndef DMA_MQDC
  mqdc32_map_segdata(MQDC32_MAPN1);
#endif

#ifdef DMA_MQDC
  mqdc32_map_dma_segdata(MQDC32_MAPN1, MQDC32_MAPN1);
#endif

  end_segment();

  /* readout for LaBr+plastic */
  init_segment(MKSEGID(RCNPEN,F3,(NAIE+1),MQDC32));
#ifndef DMA_MQDC
  mqdc32_map_segdata(MQDC32_MAPN2);
#endif

#ifdef DMA_MQDC
  mqdc32_map_dma_segdata(MQDC32_MAPN2, MQDC32_MAPN2);
#endif

  end_segment();

  mqdc32_map_clear(MQDC32_MAPN1);
  mqdc32_map_clear(MQDC32_MAPN2);
#endif

  /*readout of V1190 */
#ifdef USE_1190
  init_segment(MKSEGID(RCNPEN,F3,SSDT,V1190));
  v1X90_map_dma_segdata(V1190_MAPN);
  end_segment(); 
#endif

}
