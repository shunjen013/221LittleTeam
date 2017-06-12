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
#include <malloc.h>
#define ITERATION 1

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

int createfile(unsigned long long int size) {
    //int fp = open("/home/yen-ting/data_disk/myfile", O_RDWR|O_TRUNC|O_CREAT|O_DIRECT|O_SYNC);
    //int fp = open("/home/yen-ting/data_disk/myfile", O_RDWR|O_TRUNC|O_CREAT);
    int fp = open("file", O_RDWR|O_TRUNC|O_CREAT|O_DIRECT|O_SYNC);
    srand(time(NULL));
    char* c = (char*) memalign(sizeof(char)*4096, sizeof(char)*1024*1024);
    for(unsigned long long int i=0; i<size/(1024*1024); i++) {
        for(int j=0; j<1024*1024; j++) {
            *(c+j) = (char)(rand()%26 + 'a');
        } 
        write(fp, c, sizeof(char)*1024*1024); 
    }
    free(c);
    close(fp);
    return fp;
}


void contention(int size) {
    int fp[size];
    int pids[size];
    for(int i=0; i<size; i++) {
        char str[2];
        sprintf(str, "%d",i);
        fp[i] = open(str, O_DIRECT|O_RDWR|O_SYNC);
        //printf("%d \n", fp[i]);
    }
    unsigned long long int t1, t2;
    unsigned long long int T = 0;
    int file_size = pow(2, 22);
    int count = 0;
    int b_s = 4096;
    int iteration = 1000;
    start(&t1);
    for (int i=0; i<size ; ++i){
        if((pids[i] = fork())<0){
            perror("fork");
            abort();
        }
        else if (pids[i] == 0 ){
            unsigned long long int time1, time2;
            int total = 0;
            unsigned long long int s = 0;
            unsigned long long int t_s = 0;
            char* buffer = (char*) memalign(b_s, b_s);
            start(&time1);
            for(total=0; total<iteration; total++) {
                ssize_t gap = read(fp[i], buffer, b_s);
                s += gap;
                t_s += gap;
                if(s >=  file_size) s = 0;
            }
            end(&time2);
            free(buffer);
            exit(0);
        }
    }
    pid_t wpid;
    int status = 0;
    while((wpid = wait(&status)) > 0);
    end(&t2);
    T = (t2 - t1) / iteration;
    printf("%llu \n", T);
    for(int i=0; i<size; i++) {
        close(fp[i]);
    }
    int fd;
    char* data = "3";
    sync();
    fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
    write(fd, data, sizeof(char));
    close(fd);
}

int main(int argc, const char * argv[])
{
    //createfile(pow(2, 25));
    for(int i=1; i<17; i++) {
        contention(i);
    }
}

