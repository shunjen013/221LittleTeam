#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "utility.h"
#define ITERATION 1000

int p2[2], p3[2];
unsigned long long int t1, t2;
char buffer[10] = "";
char msg[10] = ""; 

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
void processContextswitch(int iteration)
{
    unsigned long long int time1, time2;
    unsigned long long int record[iteration];    
    unsigned long long int res[iteration];
    unsigned long long int mean, var;
    unsigned long long int ans;      
    int idx, count;
    int status = 0;
    pid_t pid, wpid;
    int p0[2], p1[2];
    for(idx=0, count=0; idx < iteration; ++idx) {
        if(pipe(p0)) continue;
        if(pipe(p1)) continue;
        pid = fork(); 
        if(pid > 0) { 
            close(p0[1]);
            close(p1[0]); 
            time1 = time2 = 0;            
            start(&time1);
            write(p1[1], msg, sizeof(msg));                
            read(p0[0], buffer, sizeof(buffer));      
            end(&time2);   
            wait(&status);                        
        }
        else if(pid == 0) {
            close(p0[0]);
            close(p1[1]);
            read(p1[0], buffer, sizeof(buffer));           
            write(p0[1], msg, sizeof(msg));                               
            _exit(0);              
        }
        else {
            exit(1);
        }
        close(p0[0]);
        close(p0[1]);      
        close(p1[0]);
        close(p1[1]);        
        if(time2 > time1 && time1!=0 && time2!=0) {
            record[count++] = (time2 - time1)/2;   
        }      
    }   
    mean = computeMean(record, count);
    var = computeVarience(record, count, mean);
    ans = computeFilter(record, count, mean, var, res, &count);
    printf("JustReading: %llu\n%d\n%llu\n%llu\n", ans, count, mean, var);
}  
void* send_1(void *ptr) {      
    t1 = t2 = 0;     
    start(&t1);
    write(p3[1], msg, sizeof(msg));     
    read(p2[0], buffer, sizeof(buffer));    
    end(&t2);
    return 0;     
}
void* send_2(void *ptr) {                 
    read(p3[0], buffer, sizeof(buffer));        
    write(p2[1], msg, sizeof(msg));         
    return 0;    
}
void threadContextswitch(int iteration)
{
    unsigned long long int time1, time2;
    unsigned long long int record[iteration];    
    unsigned long long int res[iteration];
    unsigned long long int mean, var;
    unsigned long long int ans;      
    int idx, count;
    for(idx=0, count=0; idx < iteration; ++idx) {   
        pthread_t cThread1, cThread2;
        if(pipe(p2)) continue;
        if(pipe(p3)) continue;       
        pthread_create(&cThread1, NULL, send_1, NULL);
        pthread_create(&cThread2, NULL, send_2, NULL);            
        pthread_join(cThread1, NULL);
        pthread_join(cThread2, NULL);   
        close(p2[0]);
        close(p2[1]);      
        close(p3[0]);
        close(p3[1]);             
        if(t2 > t1 && t1!=0 && t2!=0) {
            record[count++] = (t2 - t1)/2;   
        }     
    }   
    mean = computeMean(record, count);
    var = computeVarience(record, count, mean);
    ans = computeFilter(record, count, mean, var, res, &count);
    printf("JustReading: %llu\n%d\n%llu\n%llu\n", ans, count, mean, var);
}
int main(int argc, const char * argv[])
{
    processContextswitch(ITERATION);
    threadContextswitch(ITERATION); 
}

