#include "defs.h"

double findSubGraphs(graph* G, edge* maxIntWtList, int maxIntWtListSize) 
{
  mcsim_skip_instrs_begin();
  VERT_T* S;    
  LONG_T *start;
  char* visited;
  LONG_T *pSCount;
#ifdef _OPENMP
  omp_lock_t* vLock;
#endif
  
  LONG_T phase_num, numPhases;
  LONG_T count;
  
  double elapsed_time = get_seconds();
  
  numPhases = SubGraphPathLength + 1;
  
#ifdef _OPENMP 
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
#endif
    
  VERT_T *pS, *pSt;
  LONG_T pCount, pS_size;
  LONG_T v, w, search_num;
  int tid, nthreads;
  
  LONG_T j, k, vert, n;
  
#ifdef _OPENMP    
  LONG_T i;
  tid = omp_get_thread_num();
  nthreads = omp_get_num_threads();
#else
  tid = 0;
  nthreads = 1;
#endif
  
  n = G->n;
  
  pS_size = n/nthreads + 1;
  pS = (VERT_T *) malloc(pS_size*sizeof(VERT_T));
  assert(pS != NULL);
  
  if (tid == 0) {  
    S = (VERT_T *) malloc(n*sizeof(VERT_T));	
    visited = (char *) calloc(n, sizeof(char));
    start = (LONG_T *) calloc((numPhases+2), sizeof(LONG_T));
    pSCount = (LONG_T *) malloc((nthreads+1)*sizeof(LONG_T));
#ifdef _OPENMP
    vLock = (omp_lock_t *) malloc(n*sizeof(omp_lock_t));
#endif
  }
  
#ifdef _OPENMP    
#pragma omp barrier
  
#pragma omp for
  for (i=0; i<n; i++) {
    omp_init_lock(&vLock[i]);
  }
#endif
  mcsim_skip_instrs_end();
  
  for (search_num=0; search_num<maxIntWtListSize; search_num++) {
    mcsim_skip_instrs_begin();
    
#ifdef _OPENMP
#pragma omp barrier
#endif
    /* Run path-limited BFS in parallel */
    
    if (tid == 0) {
      free(visited);
      visited = (char *) calloc(n, sizeof(char));
      S[0] = maxIntWtList[search_num].startVertex;
      S[1] = maxIntWtList[search_num].endVertex;
      visited[S[0]] = (char) 1;
      visited[S[1]] = (char) 1;
      count = 2;
      phase_num = 1;
      start[0] = 0;
      start[1] = 1;
      start[2] = 2;
    }
    
    mcsim_skip_instrs_end();
    
#ifdef _OPENMP
#pragma omp barrier
#endif
    
    while (phase_num <= SubGraphPathLength) {
      
      pCount = 0;     
      
#ifdef _OPENMP
#pragma omp for
#endif
      for (vert=start[phase_num]; vert<start[phase_num+1]; vert++) {
	
	v = S[vert];
  #ifdef PERSISTENT
	mcsim_skip_instrs_begin();
	VERT_T *undolog_pS, *redolog_pS;
	undolog_pS = (VERT_T *) malloc(pS_size*sizeof(VERT_T));
	redolog_pS = (VERT_T *) malloc(pS_size*sizeof(VERT_T));
	mcsim_skip_instrs_end();
  #endif
	
	for (j=G->numEdges[v]; j<G->numEdges[v+1]; j++) {	  
	  w = G->endV[j]; 
	  if (v == w)
	    continue;
#ifdef _OPENMP
	  int myLock = omp_test_lock(&vLock[w]);
	  if (myLock) {
#endif
	    if (visited[w] != (char) 1) { 
	      visited[w] = (char) 1;
	      if (pCount == pS_size) {
		/* Resize pS */
		pSt = (VERT_T *)malloc(2*pS_size*sizeof(VERT_T));
		memcpy(pSt, pS, pS_size*sizeof(VERT_T));
		free(pS);
		pS = pSt;
		pS_size = 2*pS_size;
	      }
        #ifdef PERSISTENT
	      mcsim_log_begin();
	      //mcsim_skip_instrs_begin();
        #ifdef UNDOLOG
	      undolog_pS[pCount] = pS[pCount];
        #endif // UNDOLOG
        #ifdef REDOLOG
	      redolog_pS[pCount] = w;
        #endif // REDOLOG
	      //mcsim_skip_instrs_end();
	      mcsim_mem_fence();	      
	      mcsim_log_end();
	      mcsim_mem_fence();
        #endif // PERSISTENT
	      pS[pCount++] = w;
	    }
#ifdef _OPENMP
	    omp_unset_lock(&vLock[w]);
	  }
#endif
	}
	
	// make sure undolog and redolog data structures are not discarded by compiler
  #ifdef PERSISTENT
	mcsim_skip_instrs_begin();
	printf("%d\n", (int)((sizeof undolog_pS) + (sizeof redolog_pS)));      
	mcsim_skip_instrs_end();
  #endif // PERSISTENT
      }
      
      
#ifdef _OPENMP
#pragma omp barrier
#endif            
      pSCount[tid+1] = pCount;
      
#ifdef _OPENMP
#pragma omp barrier
#endif            
      
      if (tid == 0) {
	pSCount[0] = start[phase_num+1];
	for(k=1; k<=nthreads; k++) {
	  pSCount[k] = pSCount[k-1] + pSCount[k];
	}
	start[phase_num+2] = pSCount[nthreads];
	count = pSCount[nthreads];
	phase_num++;
      }
      
#ifdef _OPENMP
#pragma omp barrier
#endif
      #ifdef PERSISTENT
      mcsim_skip_instrs_begin();
      VERT_T *undolog_S, *redolog_S;
      undolog_S = (VERT_T *) malloc(n*sizeof(VERT_T));
      redolog_S = (VERT_T *) malloc(n*sizeof(VERT_T));
      mcsim_skip_instrs_end();
      #endif // PERSISTENT

      for (k = pSCount[tid]; k < pSCount[tid+1]; k++) {
  #ifdef PERSISTENT
	mcsim_log_begin();
	//mcsim_skip_instrs_begin();
  #ifdef UNDOLOG
	undolog_S[k] = S[k];
  #endif // UNDOLOG
  #ifdef REDOLOG
	redolog_S[k] = pS[k-pSCount[tid]];
  #endif // REDOLOG
	//mcsim_skip_instrs_end();
	mcsim_mem_fence();
	mcsim_log_end();
	mcsim_mem_fence();
  #endif // PERSISTENT 
	S[k] = pS[k-pSCount[tid]];
      } 
      #ifdef PERSISTENT
      // make sure undolog and redolog data structures are not discarded by compiler
      mcsim_skip_instrs_begin();
      printf("%d\n", (int)((sizeof undolog_S) + (sizeof redolog_S)));      
      mcsim_skip_instrs_end();
      #endif // PERSISTENT
      
      
#ifdef _OPENMP
#pragma omp barrier
#endif
    } /* End of search */
    
    mcsim_skip_instrs_begin();
    if (tid == 0) {
      fprintf(stderr, "Search from <%ld, %ld>, number of vertices visited:"
	      " %ld\n", (long) S[0], (long) S[1], (long) count);
    }
    
  } /* End of outer loop */
  
  free(pS);
#ifdef _OPENMP    
#pragma omp barrier
  
#pragma omp for
  for (i=0; i<n; i++) {
    omp_destroy_lock(&vLock[i]);
  }
#pragma omp barrier
#endif
  
  if (tid == 0) {
    free(S);
    free(start);
    free(visited);
    free(pSCount);
#ifdef _OPENMP
    free(vLock);
#endif        
  }
  
#ifdef _OPENMP    
  }
#endif
  
  elapsed_time = get_seconds() - elapsed_time;
  mcsim_skip_instrs_end();
  
  return elapsed_time;
  
}
