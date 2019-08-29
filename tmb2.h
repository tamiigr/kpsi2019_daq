/*
  TMB2, VME memory module for uPIC TPC
*/

/* Address Map */
#define TMB2_OUTBUFF1        0x0000   /* - 0x07ff (D32) CN1 */
#define TMB2_OUTBUFF2        0x0800   /* - 0x0fff (D32) CN2 */

#define TMB2_CONTREG         0x1010   /* Control Register */
#define TMB2_VERREG          0x1018   /* Version Register */
#define TMB2_STAREG1         0x1020   /* Status Register for CN1 */
#define TMB2_STAREG2         0x1028   /* Status Register for CN2 */
#define TMB2_DEPREG1         0x1030   /* Depth Register for CN1 */
#define TMB2_DEPREG2         0x1038   /* Depth Register for CN2 */
#define TMB2_COUNTREG1        0x1040   /* Counter Register for CN1 */
#define TMB2_COUNTREG2        0x1048   /* Counter Register for CN2 */
#define TMB2_RECONTREG1      0x1050   /* Read Counter Register for CN1 */
#define TMB2_RECONTREG2      0x1058   /* Read Counter Register for CN2 */

/* Bits for Control regsiter */
#define TMB2_CONT_START      0x01
#define TMB2_CONT_STOP       0x02
#define TMB2_CONT_CRESET     0x04
#define TMB2_CONT_CLEAR      0x08
#define TMB2_CONT_ALLCLEAR   0x10
#define TMB2_CONT_CLALL1     0x20
#define TMB2_CONT_CLALL0     0x00
#define TMB2_CONT_CLINC      0x60
#define TMB2_CONT_BUFCHANGE  0x80

/* Bit Mask for version regsiter */
#define TMB2_VER_REV         0x000000ff
#define TMB2_VER_VER         0x0000ff00
#define TMB2_VER_SW1         0x000f0000
#define TMB2_VER_SIDE        0x01000000

/* Bit Mask for status regsiter */
#define TMB2_STAT_RUN         0x01
#define TMB2_STAT_DATAREADY   0x02
#define TMB2_STAT_DATAINPUT   0x04
#define TMB2_STAT_RANGEOVER   0x08
#define TMB2_STAT_BUSYCLEAR   0x10

#define TMB2_WINDOWSIZE   0x200
//#define TMB2_WINDOWSIZE   0x2000


/* function definitions */
//void tmb2_multi_map_reset(int offset, int nmap);
//void tmb2_malti_map_switchbuf(int offset, int nmap);
//void tmb2_multi_map_start(int offset, int nmap);
//void tmb2_multi_map_stop(int offset, int nmap);
//void tmb2_multi_map_clear(int offset, int nmap);
//void tmb2_multi_map_setdepth(int offset, int ibuf, short depth, int nmap);
//int  tmb2_multi_map_segdata(int offset, int icn, int depth, int nmap);
//int  tmb2_multi_map_readstat(int offset, int icn, int nmap);
//int  tmb2_multi_map_readdepth(int offset, int icn, int nmap);
//int  tmb2_multi
