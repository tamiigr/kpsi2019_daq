#include "babirldrvcaen.h"

int v7XX_segdata(unsigned long maddr){
  int wordcnt;

  wordcnt = 0;
  vread32(maddr+V7XX_OUTBUFF,(long *)(data+mp));
  mp += 2;
  segmentsize += 2;
  wordcnt++;

  if((data[mp-1] & V7XX_TYPE_MASK_S) == V7XX_HEADER_BIT_S){
    while(wordcnt < 34){
      /* vread32(maddr+V775_OUTBUFF+wordcnt*32,(long *)(data+mp)); */
      vread32(maddr+V7XX_OUTBUFF,(long *)(data+mp));
      mp += 2;
      segmentsize += 2;
      wordcnt++;
      if((data[mp-1] & (V7XX_TYPE_MASK_S)) != V7XX_DATA_BIT_S){
        break;
      }
    }
  }

  return segmentsize;
}


int v7XX_segdata2(unsigned long maddr){
  int wordcnt;
  long tdata;

  wordcnt = 0;
  vread32(maddr+V7XX_OUTBUFF,&tdata);

  if((tdata & V7XX_TYPE_MASK) != V7XX_HEADER_BIT){
    return 0;
  }
  data[mp++] = (tdata)&0xffff;
  data[mp++] = (tdata >> 16)&0xffff;
  segmentsize += 2;
  wordcnt++;

  while(wordcnt < 34){
    vread32(maddr+V7XX_OUTBUFF,(long *)(data+mp));
    mp += 2;
    segmentsize += 2;
    wordcnt++;
    if((data[mp-1] & (V7XX_TYPE_MASK_S)) != V7XX_DATA_BIT_S){
      break;
    }
  }

  return segmentsize;
}

/* int v7XX_dmasegdata(unsigned long maddr, int rsize){ */
/*   /\* rsize : nanko data wo yomuka (long word wo nanko ka) *\/ */
/*   int wordcnt, csize; */
/*   int staadr, stoadr, staflag, vsize; */
/*   volatile int dmadelay, dmaflag, loop; */

/*   wordcnt = 0; */
/*   dmaflag = 0; */

/*   csize = rsize * 4;  /\* long word -> char size *\/ */
/*   vme_dma_vread32_start(maddr, csize); */
/*   delay_us();  /\* delay about 1us *\/ */
/*   dmadelay = 0; */
/*   for(dmadelay=0;dmadelay<1000;dmadelay++){ */
/*     if(vme_dma_vread32_store((char *)dmadata,csize)){ */
/*       dmadelay = 5555; */
/*       dmaflag = 1; */
/*     }else{ */
/*       delay_us(); */
/*     } */
/*   } */

/*   staadr = 0; stoadr = 0; */
/*   staflag = 0; vsize = 0; */

/*   if(dmaflag){ */
/*     for(loop=0;loop<rsize;loop++){ */
/*       if((dmadata[loop] & V7XX_TYPE_MASK) == V7XX_HEADER_BIT){ */
/*         staadr = loop; */
/*         staflag = 1; */
/*         //}else if(staflag && */
/*         //     (dmadata[loop] & V7XX_TYPE_MASK) == V7XX_DATA_BIT){ */
/*       }else if(staflag && */
/*                (((dmadata[loop] & V7XX_TYPE_MASK) == V7XX_EOB_BIT) || */
/*                 ((dmadata[loop] & V7XX_TYPE_MASK) == V7XX_ERROR_BIT))){ */
/*         stoadr = loop; */
/*         break; */
/*       }else if(!staflag && loop > 1){ */
/*         stoadr = 0; */
/*         break; */
/*       } */
/*     } */
/*     if(loop >= rsize){ */
/*       stoadr = rsize - 1; */
/*     } */
/*     vsize = stoadr - staadr + 1; */

/*     memcpy((char *)(data+mp),(char *)(dmadata+staadr),vsize*4); */

/*     wordcnt += vsize; */
/*     segmentsize += vsize*2; */
/*     mp += vsize*2; */
/*   }else{ */
/*     data[mp++] = dmadelay & 0xffff; */
/*     data[mp++] = 0x0600; */
/*     wordcnt++; */
/*     segmentsize += 2; */
/*   } */

/*   return segmentsize; */
/* } */

void v7XX_clear(unsigned long maddr){
  short sval;

  sval = 0x04;

  vwrite16(maddr + V7XX_BIT_SET2, &sval);
  vwrite16(maddr + V7XX_BIT_CLE2, &sval);

}

#ifdef UNIV
int v7XX_multi_map_segdata(unsigned long moff, int n){
  int wordcnt;

  wordcnt = 0;
  univ_map_read32(moff+V7XX_OUTBUFF,(long *)(data+mp), n);
  mp += 2;
  segmentsize += 2;
  wordcnt++;

  if((data[mp-1] & V7XX_TYPE_MASK_S) == V7XX_HEADER_BIT_S){
    while(wordcnt < 34){
      univ_map_read32(moff+V7XX_OUTBUFF,(long *)(data+mp), n);
      mp += 2;
      segmentsize += 2;
      wordcnt++;
      if((data[mp-1] & (V7XX_TYPE_MASK_S)) != V7XX_DATA_BIT_S){
        break;
      }
    }
  }

  return segmentsize;
}

int v7XX_map_segdata(int n){
  return v7XX_multi_map_segdata(0, n);
}


void v7XX_multi_map_clear(unsigned long moff, int n){
  short sval;

  sval = 0x04;

  univ_map_write16(moff + V7XX_BIT_SET2, &sval, n);
  univ_map_write16(moff + V7XX_BIT_CLE2, &sval, n);
}

void v7XX_map_clear(int n){
  v7XX_multi_map_clear(0, n);
}

/* added by furuno on 17/06/20 */
void v7XX_map_int_level(short level, int n){
  univ_map_write16(V7XX_INT_REG1, &level, n);  
}

void v7XX_map_event_trigger_register(short val, int n){
  univ_map_write16(V7XX_EVT_TRIG_REG, &val, n);
}

/* added by inaba on 17/07/13 */
void v7XX_map_set_interrupt(short level, short n, int nmap){
  univ_map_write16(V7XX_INT_REG1, &level, nmap);
  univ_map_write16(V7XX_EVT_TRIG_REG, &n, nmap);
}

void v775_map_crate_sel(short sval, int nmap){
  univ_map_write16(V7XX_CRATE_SEL, &sval, nmap);
} 

void v775_map_conf1(short sval, int nmap){
  univ_map_write16(V7XX_BIT_SET2, &sval, nmap);
}

void v775_map_conf2(short sval, int nmap){
  univ_map_write16(V7XX_BIT_CLE2, &sval, nmap);
}

void v775_map_full_scale_range(short sval, int nmap){
    univ_map_write16(V775_FULL_SCA_RANG, &sval, nmap);
}

void v775_map_set_cnt_reg(short sval, int nmap){
  univ_map_write16(0x1010, &sval, nmap);
}

void v775_map_evt_cnt_rst(int nmap){
  short sval;
  sval = 0x1;
  univ_map_write16(V7XX_EVT_CNT_RST, &sval, nmap);
}


//int v1X90_multi_map_dma_segdata(int dman, int n){
//  int dmacnt;
//
//  univ_map_read32(V1X90_EVT_FIFO, (long *)(&dmacnt), n);
//  dmacnt = (dmacnt & 0x0000ffff) * 4;
//  return univ_dma_segdata(dmacnt, dman);
//}

// furuno modify
int v1X90_multi_map_dma_segdata(int ndma, int offset, int n){
  int dmacnt;

  univ_map_read32((offset+V1X90_EVT_FIFO), (long *)(&dmacnt), n);
  dmacnt = (dmacnt & 0x0000ffff) * 4;
  if(dmacnt > DMASIZE) dmacnt=DMASIZE;
  return univ_dma_segdata(dmacnt, ndma);
}

int v1X90_map_dma_segdata(int n){
  /* ndma = n */
  return v1X90_multi_map_dma_segdata(n, 0, n);
}

void v1X90_multi_map_clear(unsigned long moff, int n){
  short sval = V1X90_SOFT_CLEAR;

  univ_map_write16(moff+V1X90_SOFT_CLEAR, &sval, n); 
}

void v1X90_map_clear(int n){
  v1X90_multi_map_clear(0, n);
}

void v1X90_multi_map_intlevel(short level, unsigned long moff, int n){
  univ_map_write16(moff+V1X90_INT_LEVEL, &level, n);
}

void v1X90_map_intlevel(short level, int n){
  v1X90_multi_map_intlevel(level, 0, n);
}

void v1X90_multi_map_evt_reset(unsigned long moff, int nmap){
  short sval;
  sval=0x1;
  univ_map_write16(moff+V1X90_EVT_RESET, &sval, nmap);
}

void v1X90_multi_map_almost_full(short sval, unsigned long moff, int nmap){
  univ_map_write16(moff+V1X90_ALMOST_FULL, &sval, nmap);
}

void v1X90_multi_map_cnt_reg(short sval, unsigned long moff, int nmap){
    univ_map_write16(moff+V1X90_CNT_REG, &sval, nmap);
}

// function for v977 written by Inaba
void v977_map_outset(short val, int n){
  univ_map_write16(V977_OUT_SET, &val, n);
}

void v977_map_int_level(short val, int n){
  univ_map_write16(V977_INT_LEVEL, &val, n);
}

void v977_map_int_mask(short val, int n){
  univ_map_write16(V977_INT_MASK, &val, n);
}

void v977_map_out_clear(int n){
  short sval;
  sval=1;
  univ_map_write16(V977_OUT_CLE, &sval, n);
}


/* functions for V830 scaler */
void v830_map_soft_clear(int nmap){
  short sval;
  sval=1;
  univ_map_write16(V830_SOFT_CLE, &sval, nmap);  
}

int v830_map_ridf_nscaler(int nmap){
  int i;
  for(i=0; i<V830_NCH; i++){
    univ_map_read32(V830_COUNTER_0+i*4, (long *)(data+mp), nmap);
    mp+=2;
  }
  scrsize+=2*V830_NCH;
  if(eventsize>0) eventsize+=2*V830_NCH;
  return scrsize;
}


/* functions for V260 */
void v260_map_clear(int nmap){
  short sval;
  sval=1;
  univ_map_write16(V260_CLEAR, &sval, nmap);  
}


int v260_map_ridf_nscaler(int nmap){
  int i;
  for(i=0; i<V260_NCH; i++){
    univ_map_read32(V260_COUNTER_0+i*4, (long *)(data+mp), nmap);
    mp+=2;
  }
  scrsize+=2*V260_NCH;
  if(eventsize>0) eventsize+=2*V260_NCH;
  return scrsize;
}

#endif
