#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "utility.h"
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <asm/unistd.h>
#define ITERATION 1000000

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
void procedureCall_0() {}
void procedureCall_1(int a) {}
void procedureCall_2(int a, int b) {}
void procedureCall_3(int a, int b, int c) {}
void procedureCall_4(int a, int b, int c, int d) {}
void procedureCall_5(int a, int b, int c, int d, int e) {}
void procedureCall_6(int a, int b, int c, int d, int e, int f) {}
void procedureCall_7(int a, int b, int c, int d, int e, int f, int g) {}

static void measureReadingAndProcedureCallOverhead(int iteration)
{
    unsigned long long int time1, time2;
    unsigned long long int *record = 
        (unsigned long long int*) malloc(sizeof(unsigned long long int)*iteration);   
    unsigned long long int *res = 
        (unsigned long long int*) malloc(sizeof(unsigned long long int)*iteration);
    unsigned long long int mean, var, ans;
    int idx, count;

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        end (&time2);
        record[idx] = time2 - time1;
        /*printf("%llu\n", record[idx]); */       
    }
    printf("JustReading:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_0();
        end (&time2);
        record[idx] = time2 - time1;
    }
    printf("procedure_0:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_1(0);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_1:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_2(0, 1);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_2:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_3(0, 1, 2);
        end (&time2);
        record[idx] = time2 - time1;
    }  
    printf("procedure_3:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_4(0, 1, 2, 3);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_4:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_5(0, 1, 2, 3, 4);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_5:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_6(0, 1, 2, 3, 4, 5);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_6:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        procedureCall_7(0, 1, 2, 3, 4, 5, 6);
        end (&time2);
        record[idx] = time2 - time1;
    }   
    printf("procedure_7:");
    ans = filterByVarience(record, iteration, res, &count);

    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        end (&time2);
        record[idx] = time2 - time1;
    }    
    printf("JustReading:");
    ans = filterByVarience(record, iteration, res, &count);

    struct timeval GG;
    int pid;
    for (idx = 0; idx < iteration; ++idx) {
        start (&time1);
        //gettimeofday(&GG, NULL);
        //getuid();
	    //getpid();
	    pid = syscall(__NR_getpid);
	    end (&time2);
        record[idx] = time2 - time1;
    }
    printf("SystemCall:");
    ans = filterByVarience(record, iteration, res, &count);

    free (record);
    free (res);
} 
int main(int argc, const char * argv[])
{
    measureReadingAndProcedureCallOverhead(ITERATION);
    measureReadingAndProcedureCallOverhead(ITERATION);
    measureReadingAndProcedureCallOverhead(ITERATION);
}

