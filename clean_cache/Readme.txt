This benchmark is used to clean the cache before an experiment to eliminate the
influence of previous experiment.
It can also perform assorted data access patterns, such as streaming,
   thrashing and recency-friendly.

Usage: clean_cache [-s size] [-l loops]

size: the size of an array.
loops: the number of iteration. In each iteration, an element of the arrage
will be increased by 1.
