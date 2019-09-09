#define SEGIDGEN(device,focal,detector,module) ((((device<<6 | focal) << 6) | detector)<<8 | module)    

#include "./segidlist.h"
#define EFN 94

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
#define A24 0x39

#define USE_1190
#define USE_MADC
#define USE_MQDC
#define USE_V260

#define DMA_MADC
#define DMA_MQDC

/* Definitions for LUPO */
#define LUPOADDR 0x30000000
#define LUPOMAPN 8

/* Definitions for MADC32 */
#define MADC32_BASE_ADR 0x32010000
#define MADC32_MAP_SIZE 0xa000
#define MADC32_MAPN     1

/* Definitions for MQDC32 */
#define MQDC32_MAP_SIZE  0xa000
#define MQDC32_BASE_ADR1 0x33000000
#define MQDC32_MAPN1     2
#define MQDC32_BASE_ADR2 0x34000000
#define MQDC32_MAPN2     4

/* Definitions for V1190 */
#define V1190_BASE_ADR  0x11110000 // now used for KPSI2019
#define V1190_MAP_SIZE  0x00010000
#define V1190_MAPN 3

/* Definitions for V260 scaler */
#define V260_MAPN     6
#define SCAID        10
#define V260_ADR     0x666600
#define V260_REGSIZE   0x2000

