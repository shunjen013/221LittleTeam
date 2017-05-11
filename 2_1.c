#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include "utility.h"

#define ITERATION 1000000
#define CACHELINE 64/sizeof(int)

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


void accessArray (unsigned long long int size) {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long[ITERATION];
    unsigned long long int* res    = new unsigned long long[ITERATION];
    unsigned long long int mean, var, ans;
    int count;
    unsigned long long int idx = 0;

    int** arr = new int*[size];
    while (idx + CACHELINE < size) {
        arr[idx] = (int*) (&arr[idx + CACHELINE]);
        idx += CACHELINE;
    }
    arr[idx] = (int*) (&arr[0]);
    int** working = (int**) (arr[0]);
    for (idx = 0; idx < ITERATION; idx ++) {
        start (&time1);
        working = (int**) (*working);
        end (&time2);
        record[idx] = time2 - time1;
    }
    ans = filterByVarience(record, ITERATION, res, &count);
 
    delete [] record;
    delete [] res;
    delete [] arr;
}         
void accessDiffSizeArray (int lo, int hi) {
    for (int idx = lo; idx <= hi; idx ++) {
        unsigned long long int siz = (unsigned long long int) pow (2, idx);
        printf ("2^%d %lld \n", idx, siz);
        accessArray (siz);
    } 
}    
int main(int argc, const char * argv[])
{
    accessDiffSizeArray (5, 25);
}

