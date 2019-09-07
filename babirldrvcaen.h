/* define */


#define v775_multi_map_segdata   v7XX_multi_map_segdata
#define v785_multi_map_segdata   v7XX_multi_map_segdata
#define v792_multi_map_segdata   v7XX_multi_map_segdata
#define v775_multi_map_clear     v7XX_multi_map_clear
#define v785_multi_map_clear     v7XX_multi_map_clear
#define v792_multi_map_clear     v7XX_multi_map_clear
#define v775_map_segdata         v7XX_map_segdata
#define v785_map_segdata         v7XX_map_segdata
#define v792_map_segdata         v7XX_map_segdata
#define v775_map_clear           v7XX_map_clear
#define v785_map_clear           v7XX_map_clear
#define v792_map_clear           v7XX_map_clear
#define v1190_map_dma_segdata    v1X90_map_dma_segdata
#define v1290_map_dma_segdata    v1X90_map_dma_segdata
#define v1190_map_clear          v1X90_map_clear
#define v1290_map_clear          v1X90_map_clear

/*
   V7XX, 32 channel TDC
*/

/* Address Map */
#define V7XX_OUTBUFF        0x0000   /* - 0x07ff (D32) */
#define V7XX_FREV           0x1000
#define V7XX_GEO_ADDR       0x1002
#define V7XX_MCST_ADDR      0x1004
#define V7XX_BIT_SET1       0x1006
#define V7XX_BIT_CLE1       0x1008
#define V7XX_INT_REG1       0x100a
#define V7XX_INT_VEC1       0x100c
#define V7XX_STS_REG1       0x100e
#define V7XX_CTRL_REG1      0x1010
#define V7XX_ADER_H         0x1012
#define V7XX_ADER_L         0x1014
#define V7XX_SS_RST         0x1016
#define V7XX_MCST_CTRL      0x101a
#define V7XX_EVT_TRIG_REG   0x1020
#define V7XX_STS_REG2       0x1022
#define V7XX_EVT_CNT_L      0x1024
#define V7XX_EVT_CNT_H      0x1026
#define V7XX_INC_EVT        0x1028
#define V7XX_INC_OFF        0x102a
#define V7XX_LOAD_TEST_REG  0x102c
#define V7XX_FCLR_WINDOW    0x102e
#define V7XX_BIT_SET2       0x1032
#define V7XX_BIT_CLE2       0x1034
#define V7XX_WMEM_TEST      0x1036
#define V7XX_MEM_TEST_H     0x1038
#define V7XX_MEM_TEST_L     0x103a
#define V7XX_CRATE_SEL      0x103c
#define V7XX_TEST_EVT       0x103e
#define V7XX_EVT_CNT_RST    0x1040
#define V7XX_RTEST          0x1064
#define V7XX_SW_COMM        0x1068
#define V7XX_SLIDE_CONST    0x106a
#define V7XX_AAD            0x1070
#define V7XX_BAD            0x1072
#define V7XX_THRESH         0x1080 /* - 0x10BF */
#define V775_FULL_SCA_RANG  0x1060

/* Bits */
#define V7XX_TYPE_MASK      0x06000000
#define V7XX_HEADER_BIT     0x02000000
#define V7XX_DATA_BIT       0x00000000
#define V7XX_EOB_BIT        0x04000000
#define V7XX_ERROR_BIT      0x06000000
#define V7XX_TYPE_MASK_S    0x0600
#define V7XX_HEADER_BIT_S   0x0200
#define V7XX_DATA_BIT_S     0x0000
#define V7XX_EOB_BIT_S      0x0400
#define V7XX_ERROR_BIT_S    0x0600



/* For V1190, V1290 */
/* Register */
#define V1X90_INT_LEVEL       0x100a
#define V1X90_SOFT_CLEAR      0x1016
#define V1X90_ALMOST_FULL     0x1022
#define V1X90_EVT_FIFO        0x1038
#define V1X90_EVT_RESET       0x1018
#define V1X90_CNT_REG         0x1000
/* Bit */
#define V1X90_SOFT_CLEAR_BIT  1

/* v977 */
#define V977_INP_SET        0x0000
#define V977_INP_MASK       0x0002
#define V977_INP_READ       0x0004
#define V977_SHIT_READ      0x0006
#define V977_MHIT_READ      0x0008
#define V977_OUT_SET        0x000a
#define V977_OUT_MASK       0x000c
#define V977_INT_MASK       0x000e
#define V977_OUT_CLE        0x0010
#define V977_SHIT_RCLR      0x0016
#define V977_MHIT_RCLR      0x0018
#define V977_TEST_CONT_REG  0x001a
#define V977_INT_LEVEL      0x0020
#define V977_INT_VECTOR     0x0022
#define V977_SERIAL_NUM     0x0024
#define V977_FIRM_REV       0x0026
#define V977_CONT_REG       0x0028
#define V977_DUMMY_REG      0x002a
#define V977_SOFT_RESET     0x002e

/* V830 */
#define V830_COUNTER_0      0x1000
#define V830_SOFT_CLE       0x1122
#define V830_NCH            32

/* V260 scaler */
#define V260_NCH            16
#define V260_CLEAR          0x50
#define V260_COUNTER_0      0x10
