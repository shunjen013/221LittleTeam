#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include "utility.h"

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

void file_cache() {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int ans;
    int it=0, count=0;
    for(int i=20; i<30; i++) {
        FILE *fp = fopen("myfile", "w");
        unsigned long long int size = pow(2,i);
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %d Mbytes.\n", int(pow(2,i-20)));  
        fclose(fp);
        fp = fopen("myfile", "r");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result = fread(temp, 512, size/512, fp);    
        srand(time(NULL));
        char buffer[512];
        for (it=0; it<ITERATION; ++it) {
            /*uint64_t num = 
              (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) | 
              (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) | 
              (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
              (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);  */  
            rewind(fp);          
            start (&time1);
            //fseek(fp , num/512*512%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 512, size/512, fp);
            end (&time2);
            record[it] = (time2 - time1) / int(pow(2,i-20));
        }
        ans = filterByVarience(record, ITERATION, res, &count);     
        fclose(fp);  
        free(temp);              
    }    

    for(int i=1; i<4; i++) {
        FILE *fp = fopen("myfile", "w");
        unsigned long long int size = i*pow(2,30);
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %d Gbytes.\n", i);  
        fclose(fp);
        fp = fopen("myfile", "r");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result = fread(temp, 1, size, fp);    
        srand(time(NULL));
        char buffer[512];
        for (it=0; it<ITERATION; ++it) {
            /*uint64_t num = 
              (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) | 
              (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) | 
              (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
              (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);  */  
            rewind(fp);          
            start (&time1);
            //fseek(fp , num/512*512%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 1, size, fp);
            end (&time2);
            record[it] = (time2 - time1) / int(pow(2,10)*i);
        }
        ans = filterByVarience(record, ITERATION, res, &count);     
        fclose(fp);  
        free(temp);              
    }     
}

int main(int argc, const char * argv[])
{
    file_cache();
}

