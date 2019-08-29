extern void univ_init_window(unsigned long vaddr, unsigned long size, int am, int n);
extern void univ_end_window(int n);
extern void univ_map_read16(unsigned long off, short *data, int n);
extern void univ_map_read32(unsigned long off, long *data, int n);
extern void univ_map_write16(unsigned long off, short *data, int n);
extern void univ_map_write32(unsigned long off, long *data, int n);
extern void univ_init_dma(unsigned long vaddr, unsigned long size, int n);
extern void univ_end_dma(int n);
extern int univ_dma_read(char *data, unsigned long size, int n);

int univ_dma_segdata(int size, int n){
  int csize;
  
  csize = univ_dma_read((char *)(data+mp), size, n);

  segmentsize += csize/2;
  mp += csize/2;

  return csize;
}

/* added on 17/09/01. suggested by Baba */
int univ_dma_segdata_tmb(int size, int n){
  int tsize;
  int csize;
  csize=0;

  while(size>0){
    if(size > 1024){
      tsize=1024;
    }else{
      tsize=size;
    }

    csize = univ_dma_read((char *)(data+mp), tsize, n);
    size = size - csize;
    segmentsize += csize/2;
    mp += csize/2;
  }
  return csize;
}

int univ_map_segndata32(int size, unsigned long off,int n){
  int i;

  for(i=0;i<size;i++){
    univ_map_read32(off, (long *)(data+mp), n);

    mp += 2;
    segmentsize += 2;
  }
  
  return segmentsize;
}

int univ_map_segndata16(int size, unsigned long off,int n){
  int i;

  for(i=0;i<size;i++){
    univ_map_read16(off, (short *)(data+mp), n);

    mp ++;
    segmentsize ++;
  }
  
  return segmentsize;
}
