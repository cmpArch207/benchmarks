#include "defs.h"

double getStartLists(graph* G, edge** maxIntWtListPtr, 
		     INT_T* maxIntWtListSizePtr) 
{
    
  mcsim_skip_instrs_begin();
  LONG_T *local_max, maxWeight;
  
  edge *maxIntWtList;
  LONG_T maxIntWtListSize;
  
  LONG_T *p_start, *p_end;
  double elapsed_time;
  elapsed_time = get_seconds();    
  
#ifdef _OPENMP
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
#endif    
    
  LONG_T i, j, n;
  edge* pList;
  LONG_T pCount, tmpListSize;
  int tid, nthreads;

#ifdef DIAGNOSTIC
  double elapsed_time_part;
#endif
  
#ifdef _OPENMP
  tid = omp_get_thread_num();
  nthreads = omp_get_num_threads();
#else
  tid = 0;
  nthreads = 1;
#endif
    
  n = G->n;
  
  /* Determine the maximum edge weight */
  
  if (tid == 0) {
    local_max = (LONG_T *) malloc(nthreads*sizeof(LONG_T));
  }
  
  /* Allocate memory for partial edge list on each thread */
  tmpListSize = 1000;
  pList = (edge *) malloc(tmpListSize*sizeof(edge));
  pCount = 0;
  
#ifdef _OPENMP
#pragma omp barrier
#endif
  
  local_max[tid] = -1;
  
#ifdef DIAGNOSTIC
  if (tid == 0) {
    elapsed_time_part = get_seconds();
  }
#endif
  
  mcsim_skip_instrs_end();
  
#ifdef _OPENMP    
#pragma omp for
#endif
  for (i=0; i<n; i++) {
    for (j=G->numEdges[i]; j<G->numEdges[i+1]; j++) {
      if (G->weight[j] > local_max[tid]) {
	mcsim_log_begin();
	//mcsim_skip_instrs_begin();
	LONG_T *undolog_local_max;
	//LONG_T *redolog_local_max;	
	undolog_local_max = (LONG_T *) malloc(nthreads*sizeof(LONG_T));
	//redolog_local_max = (LONG_T *) malloc(nthreads*sizeof(LONG_T));
	undolog_local_max[tid] = local_max[tid];
	//redolog_local_max[tid] = G->weight[j];
	//mcsim_skip_instrs_end();
	mcsim_mem_fence();
	mcsim_log_end();
	mcsim_mem_fence();
	
	local_max[tid] = G->weight[j];
	pCount = 0;
	
	mcsim_log_begin();
	//mcsim_skip_instrs_begin();
	edge *undolog_pList;
	//edge *redolog_pList;
	undolog_pList = (edge *) malloc(tmpListSize*sizeof(edge));
	//redolog_pList = (edge *) malloc(tmpListSize*sizeof(edge));
	undolog_pList[pCount].startVertex = pList[pCount].startVertex;
	undolog_pList[pCount].endVertex = pList[pCount].endVertex;
	undolog_pList[pCount].w = pList[pCount].w;
	undolog_pList[pCount].e = pList[pCount].e;
	//redolog_pList[pCount].startVertex = i;
	//redolog_pList[pCount].endVertex = G->endV[j];
	//redolog_pList[pCount].w = local_max[tid];
	//redolog_pList[pCount].e = j;
	//mcsim_skip_instrs_end();
	mcsim_mem_fence();
	mcsim_log_end();
	mcsim_mem_fence();

	pList[pCount].startVertex = i;
	pList[pCount].endVertex = G->endV[j];
	pList[pCount].w = local_max[tid];
	pList[pCount].e = j;
	pCount++;
	
	// make sure undolog and redolog data structures are not discarded by compiler
	mcsim_skip_instrs_begin();
	printf("%d\n", (sizeof undolog_local_max) + (sizeof undolog_pList));      
	//printf("%d\n", (sizeof redolog_local_max) + (sizeof redolog_pList));
	mcsim_skip_instrs_end();      
      } else if (G->weight[j] == local_max[tid]) {
	mcsim_log_begin();
	//mcsim_skip_instrs_begin();
	edge *undolog_pList;
	//edge *redolog_pList;
	undolog_pList = (edge *) malloc(tmpListSize*sizeof(edge));
	//redolog_pList = (edge *) malloc(tmpListSize*sizeof(edge));
	undolog_pList[pCount].startVertex = pList[pCount].startVertex;
	undolog_pList[pCount].endVertex = pList[pCount].endVertex;
	undolog_pList[pCount].w = pList[pCount].w;
	undolog_pList[pCount].e = pList[pCount].e;
	//redolog_pList[pCount].startVertex = i;
	//redolog_pList[pCount].endVertex = G->endV[j];
	//redolog_pList[pCount].w = local_max[tid];
	//redolog_pList[pCount].e = j;
	//mcsim_skip_instrs_end();
	mcsim_mem_fence();
	mcsim_log_end();
	mcsim_mem_fence();
	
	pList[pCount].startVertex = i;
	pList[pCount].endVertex = G->endV[j];
	pList[pCount].w = local_max[tid];
	pList[pCount].e = j;
	pCount++; 

	// make sure undolog and redolog data structures are not discarded by compiler
	mcsim_skip_instrs_begin();
	printf("%d\n", (sizeof undolog_pList));
	//printf("%d\n", (sizeof redolog_pList));
	mcsim_skip_instrs_end();
      }
    }
  }
  
#ifdef _OPENMP
#pragma omp barrier
#endif
  
  if (tid == 0) {
    
#ifdef DIAGNOSTIC
    if (tid == 0) {
      elapsed_time_part = get_seconds() - elapsed_time_part;
      fprintf(stderr, "Max. weight computation time: %lf seconds\n", elapsed_time_part);
    }
#endif
    
    maxWeight = local_max[0];
    
    for (i=1; i<nthreads; i++) {
      if (local_max[i] > maxWeight)
	maxWeight = local_max[i];
    }
    // free(local_max);
  }
  
#ifdef _OPENMP
#pragma omp barrier
#endif
  
  if (local_max[tid] != maxWeight) {
    pCount = 0;
  }
  
  mcsim_skip_instrs_begin();
  /* Merge all te partial edge lists */
  if (tid == 0) {
    p_start = (LONG_T *) malloc(nthreads*sizeof(LONG_T));
    p_end = (LONG_T *) malloc(nthreads*sizeof(LONG_T));
  }
  
#ifdef _OPENMP    
#pragma omp barrier
#endif
  
  p_end[tid] = pCount;
  p_start[tid] = 0;
  
#ifdef _OPENMP    
#pragma omp barrier
#endif
  
  if (tid == 0) {
    for (i=1; i<nthreads; i++) {
      p_end[i] = p_end[i-1] + p_end[i];
      p_start[i] = p_end[i-1]; 
    }
    
    maxIntWtListSize = p_end[nthreads-1];
    free(*maxIntWtListPtr);
    maxIntWtList = (edge *) malloc((maxIntWtListSize)*sizeof(edge));    
  }
  mcsim_skip_instrs_end();
  
#ifdef _OPENMP    
#pragma omp barrier
#endif
  
  for (j=p_start[tid]; j<p_end[tid]; j++) {
    mcsim_log_begin();
    //mcsim_skip_instrs_begin();
    edge *undolog_maxIntWtList;
    //edge *redolog_maxIntWtList;
    undolog_maxIntWtList = (edge *) malloc((maxIntWtListSize)*sizeof(edge));
    //redolog_maxIntWtList = (edge *) malloc((maxIntWtListSize)*sizeof(edge));
    (undolog_maxIntWtList[j]).startVertex = (maxIntWtList[j]).startVertex;
    (undolog_maxIntWtList[j]).endVertex   = (maxIntWtList[j]).endVertex;
    (undolog_maxIntWtList[j]).e           = (maxIntWtList[j]).e;
    (undolog_maxIntWtList[j]).w           = (maxIntWtList[j]).w;    
    //(redolog_maxIntWtList[j]).startVertex = pList[j-p_start[tid]].startVertex;
    //(redolog_maxIntWtList[j]).endVertex = pList[j-p_start[tid]].endVertex;
    //(redolog_maxIntWtList[j]).e = pList[j-p_start[tid]].e;
    //(redolog_maxIntWtList[j]).w = pList[j-p_start[tid]].w;
    //mcsim_skip_instrs_end();
    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    (maxIntWtList[j]).startVertex = pList[j-p_start[tid]].startVertex;
    (maxIntWtList[j]).endVertex = pList[j-p_start[tid]].endVertex;
    (maxIntWtList[j]).e = pList[j-p_start[tid]].e;
    (maxIntWtList[j]).w = pList[j-p_start[tid]].w;
    
    mcsim_skip_instrs_begin();
    printf("%d\n", (sizeof undolog_maxIntWtList));
    //printf("%d\n", (sizeof redolog_maxIntWtList));
    mcsim_skip_instrs_end();
  } 
  
  
#ifdef _OPENMP
#pragma omp barrier
#endif
  
  mcsim_skip_instrs_begin();
  free(pList);
  
  if (tid == 0) {
    free(local_max);
    free(p_start);
    free(p_end);
    *maxIntWtListPtr = maxIntWtList;
    *maxIntWtListSizePtr = maxIntWtListSize;
  }
  
#ifdef _OPENMP
  }
#endif
  
  /* Verification */
#if 0
  maxIntWtList = *maxIntWtListPtr;
  for (int i=0; i<*maxIntWtListSizePtr; i++) {
    fprintf(stderr, "[%ld %ld %ld %ld] ", maxIntWtList[i].startVertex, 
	    maxIntWtList[i].endVertex, maxIntWtList[i].e, maxIntWtList[i].w);
  }
#endif
  
  elapsed_time = get_seconds() - elapsed_time;
  mcsim_skip_instrs_end();

  return elapsed_time;
}
