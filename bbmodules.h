#define SEGIDGEN(device,focal,detector,module) ((((device<<6 | focal) << 6) | detector)<<8 | module)    

#include "./segidlist.h"
#define EFN 10

#define BBRL
//#define MAXBUFF    8192
#define MAXBUFF    16000
#define DBUFF

#define VME
#define VMEINT

#define INTLEVEL 3
#define INTVEC   0
#define UNIV

//#define DMASIZE  8192
#define DMASIZE  16000
#define REGSIZE  0xa000

#define A32 0x09
#define A16 0x29

#define USE_RPV
#define USE_1190
#define USE_MADC
#define USE_TMB2
//#define USE_V775

/* Definitions for RPV130*/
#define RPV130ADR 0xf000
#define RPV130_MAP_SIZE 0x1000
#define RPV130MAPN  3
#define OPBUSYCL    0x1
#define OPIWKCLE    0x2
#define OPIWKRST    0x4
#define OPSCASTART  0x8
#define OPSCASTOP   0x10
#define OPSCACLEAR  0x20
#define OPDAQSTART  0x40
#define OPVETO      0x80

/* Definitions for V1190s */
#define V1190_BASE_ADR  0x00050000
#define V1190_MAP_SIZE  0x00030000
#define V1190_MAPN 2  // for DMA 2-3 will be reserved
#define V1190_MAIKO_ADR 0x00050000
#define V1190_BDC1_ADR  0x00060000
#define V1190_BDC2_ADR  0x00070000

/* Definitions for MADC32 */
#define MADC32_BASE_ADR 0x32000000
#define MADC32_MAP_SIZE     0xa000
#define MADC32_MAPN     1

/* Definition for V977 */
#define V977_MAPN 4
#define V977ADR 0x20000000
#define V977REGSIZE 0x2000

/* Definition for TMB2 */
#define TMB2_MAPN    5
#define TMB2_BASEADR 0x002f0000
#define TMB2_ADR0    0x002f0000
#define TMB2_ADR1    0x00300000
#define TMB2_ADR2    0x00310000
#define TMB2_REGSIZE    0x30000
#define TMB2_NMEM    3
#define TMB2_NCN     2
#define TMB2DEPTH    0x8000
#define FADCDEPTH    256
#define TPCDEPTH     8193

static int huga[2]={1,2};

/* definition for V830 */
#define V830_MAPN     6
#define V830_NCH     32
#define SCAID        10
#define V830_ADR     0x66660000
#define V830_REGSIZE     0x2000

/* definition of V775 */
#define V775_MAPN     7
#define V775_ADR     0x00010000
#define V775_REGSIZE     0x2000
