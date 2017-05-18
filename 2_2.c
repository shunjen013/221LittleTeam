#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <cstdint>
#include "utility.h"

#define ITERATION 10
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

void getBandwidth_readutil (unsigned long long int size) {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int ans;
    int count=0, it=0;
    uint64_t* arr = new uint64_t[size];
    for(unsigned long long int i=0; i<size; i++) {
    	arr[i] = 0;
    }
    uint64_t sum = 1;
    for(it=0; it<ITERATION; it++) {
	    start(&time1);
	    for(unsigned long long int i=0; i<size; i+=16*CACHELINE) {
	    	sum += arr[i];
	    	sum += arr[i+1*CACHELINE];
	    	sum += arr[i+2*CACHELINE];
	    	sum += arr[i+3*CACHELINE];
	    	sum += arr[i+4*CACHELINE];
	    	sum += arr[i+5*CACHELINE];
	    	sum += arr[i+6*CACHELINE];
	    	sum += arr[i+7*CACHELINE];
	    	sum += arr[i+8*CACHELINE];
	    	sum += arr[i+9*CACHELINE];
	    	sum += arr[i+10*CACHELINE];  
	    	sum += arr[i+11*CACHELINE];  
	    	sum += arr[i+12*CACHELINE]; 
	    	sum += arr[i+13*CACHELINE];  
	    	sum += arr[i+14*CACHELINE];  
	    	sum += arr[i+15*CACHELINE];    	    	     	    	  	    	    	    	    	    	    	    	    	    	
	    }     
	    end(&time2);
		record[it] = time2 - time1;    
	} 

    ans = filterByVarience(record, ITERATION, res, &count); 
    delete [] record;
    delete [] res;
    delete [] arr;
}  

void getBandwidth_writeutil (unsigned long long int size) {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int ans;
    int count=0, it=0;
    uint64_t* arr = new uint64_t[size];
    for(unsigned long long int i=0; i<size; i++) {
    	arr[i] = 0;
    }
    uint64_t sum = 1;
    for(it=0; it<ITERATION; it++) {
	    start(&time1);
	    for(unsigned long long int i=0; i<size; i+=16*CACHELINE) {
	    	arr[i] = sum;
	    	arr[i+1*CACHELINE] = sum;
			arr[i+2*CACHELINE] = sum;
			arr[i+3*CACHELINE] = sum;
			arr[i+4*CACHELINE] = sum;
			arr[i+5*CACHELINE] = sum;
	    	arr[i+6*CACHELINE] = sum;
			arr[i+7*CACHELINE] = sum;
			arr[i+8*CACHELINE] = sum;
			arr[i+9*CACHELINE] = sum;
			arr[i+10*CACHELINE] = sum;
	    	arr[i+11*CACHELINE] = sum;
			arr[i+12*CACHELINE] = sum;
			arr[i+13*CACHELINE] = sum;
			arr[i+14*CACHELINE] = sum;
			arr[i+15*CACHELINE] = sum;						
	    }     
	    end(&time2);
		record[it] = time2 - time1;    
	} 

    ans = filterByVarience(record, ITERATION, res, &count); 
    delete [] record;
    delete [] res;
    delete [] arr;
}   

void getLoopOverhead (unsigned long long int size) {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int ans;
    int count=0, it=0;
    for(it=0; it<ITERATION; it++) {
	    start(&time1);
	    for(unsigned long long int i=0; i<size; i+=16*CACHELINE) {					
	    }     
	    end(&time2);
		record[it] = time2 - time1;    
	} 

    ans = filterByVarience(record, ITERATION, res, &count); 
    delete [] record;
    delete [] res;
}        

void getBandwidth(int lo, int hi) {
    for (int i=lo; i<=hi; i++) {
        unsigned long long int size = (unsigned long long int) pow (2, i);
        printf ("read_size:2^%d \n", i);
        getBandwidth_readutil(size);
    } 
    for (int i=lo; i<=hi; i++) {
        unsigned long long int size = (unsigned long long int) pow (2, i);
        printf ("write_size:2^%d \n", i);
        getBandwidth_writeutil(size);
    }     
    for (int i=lo; i<=hi; i++) {
        unsigned long long int size = (unsigned long long int) pow (2, i);
        printf ("overhead:2^%d \n", i);
        getLoopOverhead(size);
    }   
    int a = sizeof(uint64_t);
    printf("%d", a);
}    
int main(int argc, const char * argv[])
{
    getBandwidth(20, 30);
}

