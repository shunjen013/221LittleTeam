#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#define ITERATION 1000

inline void rdtscl(unsigned long long *ll)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
    *ll = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
} 
void processCreation(void)
{
    unsigned long long int time1, time2;
    unsigned long long int total = 0;
    int idx = 0;
    int status = 0;
    pid_t pid, wpid;
    for (; idx < ITERATION; ++idx) {
        rdtscl(&time1);
        pid = fork(); 
        if(pid == 0) { 
            _exit(0);
        }
        else {
            wait(&status);
        }
        rdtscl(&time2);
        total += (time2-time1);        
    }   
    printf("%lld\n", total);
} 
void* work(void *ptr) {
}
void threadCreation(void)
{
    unsigned long long int time1, time2;
    unsigned long long int total = 0;
    int idx = 0;
    for (; idx < ITERATION; ++idx) {
        rdtscl(&time1);     
        pthread_t cThread;
        pthread_create(&cThread, NULL, work, NULL);
        pthread_join(cThread, NULL);
        rdtscl(&time2);
        total += (time2-time1);        
    }   
    printf("%lld\n", total);
}
int main(int argc, const char * argv[])
{
    processCreation();
    threadCreation(); 
}

