#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include "utility.h"
    

inline void start(unsigned long long *ll)
{
    unsigned int lo, hi;                     
    asm volatile ("cpuid\n\t"
      "rdtsc\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      : "=r" (hi), "=r" (lo)
      :: "%rax", "%rbx", "%rcx", "%rdx");
    *ll = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}
inline void end(unsigned long long *ll)
{
    unsigned int lo, hi;                     
    asm volatile ("rdtscp\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      "cpuid\n\t"
      : "=r" (hi), "=r" (lo)
      :: "%rax", "%rbx", "%rcx", "%rdx");
    *ll = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}

int main(int argc, char ** argv) {
  int fd = open(argv[1], O_RDONLY);
  unsigned long long int time1, time2;
  struct stat stats;
  fstat(fd, &stats);
  posix_fadvise(fd, 0, stats.st_size, POSIX_FADV_DONTNEED);
  char * map = (char *) mmap(NULL, stats.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    perror("Failed to mmap");
    return 1;
  }
  int result = 0;
  int i;
  unsigned long long int* arr_time = new unsigned long long int [100];
  unsigned long long int* inliers = new unsigned long long int[100];
  int numInliers = 0;
  int idx = 0;
  unsigned long long int ans;

  for (i = 0; i < stats.st_size; i = i+4096*128) {
    start(&time1);
    result = map[i];
    end(&time2);
    if(idx < 100){
        arr_time[idx] = time2 - time1;
        idx++;
    }
    else
        break;
  }
  
  for (i = 0 ; i < idx; i++){
    printf("%d:%llu\n", i, arr_time[i]);
  }
  
  ans = filterByVarience(arr_time, idx, inliers, &numInliers); 
  delete []arr_time;
  delete []inliers;
  munmap(map, stats.st_size);
  return result;
}
