void sca(void){
#ifdef USE_V260
  init_ncscaler(SCAID);
  v260_map_ridf_nscaler(V260_MAPN);
  end_ncscaler32();
#endif
}

void scrinit(void){

}
