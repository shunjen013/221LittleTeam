#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include "utility.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define NUM_CHILD_PROC 4
#define ITERATION 10

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

void readFile(){
    return;
}

int main(int argc, char * argv[]){
    unsigned long long int time1, time2;
    pid_t pids[NUM_CHILD_PROC];
    int i;
    int count = NUM_CHILD_PROC;
    unsigned long long int interval;
    for (i = 0 ; i < NUM_CHILD_PROC-1 ; ++i){
        if((pids[i] = fork())<0){
            perror("fork");
            abort();
        }
        else if (pids[i] == 0 ){
            readFile();             //TO BE FILLED
            exit(0);
        }
    }
    //last child process, measure the time
    
    if((pids[i] = fork())<0){
            perror("fork");
            abort();
        }
        else if (pids[i] == 0 ){
            start(&time1);
            readFile();             //TO BE FILLED
            end(&time2);
            interval = time2 - time1;
            exit(0);
    }
    //printf("i:%d, pid:%ld\n", i, (long)pids[i]);
    int status;
    pid_t pid;
    while( count > 0){
        pid = wait(&status);
        printf("Child with PID %ld exited, status:0x%x.\n", (long)pid, status);
        --count;
    }
    printf("%llu \n", interval);
}