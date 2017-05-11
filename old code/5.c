#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define ITERATION 100000

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


static void measureContextSwitching(int iteration)
{

    unsigned long long int time1, time2;
    unsigned long long int total = 0;
    int idx = 0;    
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        for (idx = 0; idx < iteration; idx ++) {
            read(pipefd[0], &buf, 1);
            write(pipefd[1], &buf, 1);
        }
        _exit(EXIT_SUCCESS);

    } else {
        for (idx = 0; idx < iteration; idx ++) {
            start(&time1);
            write(pipefd[1], "a", 1);
            read(pipefd[0], &buf, 1);
            end(&time2);
            total += (time2-time1);
        }
        printf("%lld\n", total);
        exit(EXIT_SUCCESS);
    }
}


int main(int argc, const char * argv[])
{
    measureContextSwitching(ITERATION);
}

