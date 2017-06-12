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

void file_cache() {
    unsigned long long int time1, time2;
    unsigned long long int* record = new unsigned long long int[ITERATION];
    unsigned long long int* res    = new unsigned long long int[ITERATION];
    unsigned long long int ans;
    int it=0, count=0;
    for(int i=20; i<30; i++) {
        FILE *fp = fopen("myfile", "wr");
        unsigned long long int size = pow(2,i);
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %d Mbytes.\n", int(pow(2,i-20)));  
        fclose(fp);
        fp = fopen("myfile", "r");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result = fread(temp, 4096, size/4096, fp);    
        srand(time(NULL));
        char buffer[4096];        
        rewind(fp); 
        for (it=0; it<ITERATION; ++it) {
            unsigned long long int num = 
              (((unsigned long long int) rand() <<  0) & 0x000000000000FFFFull) | 
              (((unsigned long long int) rand() << 16) & 0x00000000FFFF0000ull) | 
              (((unsigned long long int) rand() << 32) & 0x0000FFFF00000000ull) |
              (((unsigned long long int) rand() << 48) & 0xFFFF000000000000ull);             
            start (&time1);
            //fseek(fp , num%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 4096, size/4096, fp);
            //fread(buffer, 4096, 1, fp);
            end (&time2);
            printf("%llu \n", time2-time1);
            record[it] = (time2 - time1) * 4096 / size;
        } 
        ans = filterByVarience(record, ITERATION, res, &count);     
        fclose(fp);  
        free(temp);               
    }    
	
    for(int i=1; i<7; i++) {
        FILE *fp = fopen("myfile", "w");
        unsigned long long int size = i*pow(2,30)/10;
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %d Gbytes.\n", i);  
        fclose(fp);
        fp = fopen("myfile", "r");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result = fread(temp, 4096, size/4096, fp);    
        srand(time(NULL));
        char buffer[4096];        
        rewind(fp); 
        for (it=0; it<ITERATION; ++it) {
            unsigned long long int num = 
              (((unsigned long long int) rand() <<  0) & 0x000000000000FFFFull) | 
              (((unsigned long long int) rand() << 16) & 0x00000000FFFF0000ull) | 
              (((unsigned long long int) rand() << 32) & 0x0000FFFF00000000ull) |
              (((unsigned long long int) rand() << 48) & 0xFFFF000000000000ull);             
            start (&time1);
            //fseek(fp , num%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 4096, size/4096, fp);
            //fread(buffer, 4096, 1, fp);
            end (&time2);
            printf("%llu \n", time2-time1);
            record[it] = (time2 - time1) * 4096 / size;
        } 
        ans = filterByVarience(record, ITERATION, res, &count);     
        fclose(fp);  
        free(temp);         
    }     
    for(int i=60; i<75; i++) {
        FILE *fp = fopen("myfile", "w");
        unsigned long long int size = i*pow(2,30)/10;
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %f Gbytes.\n", double(i)/10);  
        fclose(fp);
        fp = fopen("myfile", "r");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result = fread(temp, 4096, size/4096, fp);    
        srand(time(NULL));
        char buffer[4096];        
        rewind(fp); 
        for (it=0; it<ITERATION; ++it) {
            unsigned long long int num = 
              (((unsigned long long int) rand() <<  0) & 0x000000000000FFFFull) | 
              (((unsigned long long int) rand() << 16) & 0x00000000FFFF0000ull) | 
              (((unsigned long long int) rand() << 32) & 0x0000FFFF00000000ull) |
              (((unsigned long long int) rand() << 48) & 0xFFFF000000000000ull);             
            start (&time1);
            //fseek(fp , num%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 4096, size/4096, fp);
            //fread(buffer, 4096, 1, fp);
            end (&time2);
            printf("%llu \n", time2-time1);
            record[it] = (time2 - time1) * 4096 / size;
        } 
        ans = filterByVarience(record, ITERATION, res, &count);     
        fclose(fp);  
        free(temp);         
    }  
 	   
}

void file_read() {
    FILE *f = fopen("myfile", "w");
    unsigned long long int size = pow(2,33);
    fflush(f);
    ftruncate(fileno(f), size);
    fclose(f);   
    int fp = open("myfile", O_DIRECT|O_RDWR);
    unsigned long long int block_num = 1;
    unsigned long long int s = 0;
    unsigned long long int time1, time2;
    for(int i=0; i<10; i++) {
        lseek64(fp, 0, SEEK_SET);
        s = 0;
        block_num = pow(2, i);
        char* buffer = (char*) memalign(4096*1024, 4096*1024*block_num);
        start(&time1);
        while(s < block_num*4096*1024) {
            ssize_t gap = read(fp, buffer+s, 4096*1024);
            s += gap;
        }
        end(&time2);
        free(buffer);
        int t = (time2-time1) / block_num;
        printf("%llu \n", s);
        printf("%llu  %d \n", block_num, t);
    }

}

void contention() {
    
}
int main(int argc, const char * argv[])
{
    //file_cache();
    file_read();
    //contention();
}

