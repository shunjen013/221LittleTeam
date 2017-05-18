#include <stdio.h>
#include <cstdint>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include "utility.h"

#define ITERATION 100000
#define CACHELINE 64/sizeof(uint64_t)

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

void accessArray (unsigned long long int size, int xstride) {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int mean, var, ans;
    int count=0, it=0;
    unsigned long long int idx = 0;
    uint64_t* arr = new uint64_t[size];
    int stride = pow(2, 8)*xstride;
    for(unsigned long long int i=0; i<size; i++) {
        arr[i] = (uint64_t)&(arr[(i+stride)%size]);
    }
    uint64_t tmp = arr[0];    
    unsigned long long int random = 0;
    for (it=0; it<ITERATION; ++it) {
        start (&time1);
        tmp = *((uint64_t*)tmp);
        end (&time2);
        record[it] = time2 - time1;
    }
    ans = filterByVarience(record, ITERATION, res, &count);
 
    delete [] record;
    delete [] res;
    delete [] arr;
}         
void accessDiffSizeArray (int lo, int hi) {
    for(int s=1; s<=6; s++) {
        for (int idx = lo; idx <= hi; idx ++) {
            unsigned long long int size = (unsigned long long int) pow (2, idx);
            printf ("size:2^%d stride:2^8*%d \n", idx, s);
            accessArray(size, pow(2,s));
        } 
    }
}    
int main(int argc, const char * argv[])
{
    accessDiffSizeArray (11, 22);
}

