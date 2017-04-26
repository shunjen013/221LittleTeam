#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "utility.h"
#define ITERATION 1000

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

void processCreation(int iteration)
{
    unsigned long long int time1, time2;
    unsigned long long int record[iteration];    
    unsigned long long int res[iteration];
    unsigned long long int mean, var;
    unsigned long long int ans;    
    int idx, count;
    int status = 0;
    pid_t pid, wpid;
    for(idx=0, count=0; idx < iteration; ++idx) {
        time1 = time2 = 0;        
        start(&time1);
        pid = fork(); 
        if(pid > 0) {
            wait(&status);
            end(&time2);
            if(time2 > time1 && time1!=0 && time2!=0) {
                record[count++] = time2 - time1;   
            } 
        }
        else if(pid == 0) { 
            _exit(0);
        }
        else {
            exit(1);
        }             
    }   
    mean = computeMean(record, count);
    var = computeVarience(record, count, mean);
    ans = computeFilter(record, count, mean, var, res, &count);
    printf("JustReading: %llu\n%d\n%llu\n%llu\n", ans, count, mean, var);
} 
void* work(void *ptr) {
    return 0;
}
void threadCreation(int iteration)
{
    unsigned long long int time1, time2;
    unsigned long long int record[iteration];    
    unsigned long long int res[iteration];
    unsigned long long int mean, var;
    unsigned long long int ans;    
    int idx, count;
    int cnt;
    for(idx=0, count=0; idx < iteration; ++idx) {
        pthread_t cThread;   
        bool flag1, flag2;
        time1 = time2 = 0;
        start(&time1);     
        flag1 = pthread_create(&cThread, NULL, work, NULL);
        flag2 = pthread_join(cThread, NULL);
        if(flag1 || flag2) continue;
        end(&time2);
        if(time2 > time1 && time1!=0 && time2!=0) {
                record[count++] = time2 - time1;   
        }      
    }   
    mean = computeMean(record, count);
    var = computeVarience(record, count, mean);
    ans = computeFilter(record, count, mean, var, res, &count);
    printf("JustReading: %llu\n%d\n%llu\n%llu\n", ans, count, mean, var);
}

int main(int argc, const char * argv[])
{
    processCreation(ITERATION);
    threadCreation(ITERATION); 
}

