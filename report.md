## Progress
For the replacement policy, I implement the LRU, random, and Tree LRU. For the prefetcher, I implement a next-line prefetcher. I'm able to log down all the statictics regarding the prefetcher.

## LRU-PF versus LRU-NoPF
The prefetcher is able to improve the performance on BaseConvolution and MatrixMultiplication by up to 7.6%. However, the prefetcher is ineffective for the other two programs. The inefficiency is due to the lack of timeliness of prefetching -- usually the next cache block is accessed in very short time, which couldn't be hidden by the prefetching.

## LRU versus Random versus Tree LRU
For three out of the four programs, LRU performs slightly better than Tree LRU, while for the other program, Tree LRU performs slightly better. For all the four programs, random replacement is worse than either LRU or Tree LRU.

## Prefetcher statictics
The accuracy of prefetcher looks bad (with the highest being 18%). However, this might be due to the lack of timeliness of prefetching. 
