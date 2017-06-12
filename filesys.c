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
    int fp = open("/home/yen-ting/data_disk/myfile", O_RDWR|O_TRUNC|O_CREAT);
    //int fp = open("myfile", O_RDWR|O_TRUNC|O_CREAT|O_DIRECT|O_SYNC);
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

void file_cache() {
    unsigned long long int block_num = 1;
    unsigned long long int s = 0;
    unsigned long long int time1, time2;
    int iteration = 5;
    unsigned long long int* record = new unsigned long long int[iteration];
    unsigned long long int* res    = new unsigned long long int[iteration];
    int it=0, count=0;
    int b_s = 4096;
    /*
    for(int i=1; i<7;  i++) {
        unsigned long long int size = i*pow(2,30);
        createfile(size);
        int fp = open("/home/yen-ting/data_disk/myfile", O_RDONLY);
        s = 0;
        char* buffer = (char*) memalign(b_s, b_s);     
        srand(time(NULL));
        start(&time1);
        int total = 0;
        unsigned long long int t_s = 0;

        while(t_s < (size/b_s)*b_s) {
            ssize_t gap = read(fp, buffer, b_s);
            t_s += gap;  
        }
        //printf("%s \n", buffer); 
        end(&time2);
        free(buffer);
        close(fp);
        int t = (time2-time1) * 4096 / t_s;
        printf("%d %d \n", i, t);
        
        int fd;
        char* data = "3";
        sync();
        fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
        write(fd, data, sizeof(char));
        close(fd);
    }     
    1 1659 
2 1670 
3 1655 
4 1642 
5 1640 
6 1644 
7 171624 

    */
    for(int i= 60; i<75;  i++) {
        unsigned long long int size = pow(2,30)/10*i;
        createfile(size);
        int fp = open("/home/yen-ting/data_disk/myfile", O_RDONLY);
        s = 0;
        char* buffer = (char*) memalign(b_s, b_s);     
        srand(time(NULL));
        start(&time1);
        int total = 0;
        unsigned long long int t_s = 0;
        /*
        for(total=0; total<1000; total++) {
            lseek64(fp, rand()%(size/b_s)*b_s, SEEK_SET);
            ssize_t gap = read(fp, buffer, b_s);
            s += gap;
            t_s += gap;
            //printf("%d \n", gap);
        }
        */
        ssize_t gap = 5;
        while(gap > 0) {
            gap = read(fp, buffer, b_s);
            t_s += gap;  
        }
        //printf("%s \n", buffer);
        end(&time2);
        free(buffer);
        close(fp);
        int t = (time2-time1) * 4096 / t_s;
        printf("%d %d \n", i, t);
        
        int fd;
        char* data = "3";
        sync();
        fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
        write(fd, data, sizeof(char));
        close(fd);
    }     
    /*
    unsigned long long int time1, time2;
    int iteration = 5;
    unsigned long long int* record = new unsigned long long int[iteration];
    unsigned long long int* res    = new unsigned long long int[iteration];
    int it=0, count=0;
    for(int i=60; i<75; i++) {
        FILE *fp = fopen("/home/yen-ting/data_disk/myfile", "w+");
        unsigned long long int size = i*pow(2,30)/10;
        fflush(fp);
        ftruncate(fileno(fp), size);
        printf("Size of myfile: %f Gbytes.\n", double(i)/10);  
        fclose(fp);
        fp = fopen("/home/yen-ting/data_disk/myfile", "r+");
        char *temp = (char*) malloc (sizeof(char)*size);
        unsigned long long int result;
        //unsigned long long int result = fread(temp, 4096, size/4096, fp);    
        char buffer[4096];        
        rewind(fp); 
        for (it=0; it<ITERATION; ++it) {   
            start (&time1);
            //fseek(fp , num%size , SEEK_SET); 
            //fread(buffer, 1, 512, fp);
            fread(temp, 4096, size/4096, fp);
            //fread(buffer, 4096, 1, fp);
            end (&time2);
            record[it] = (time2 - time1) * 4096 / size;
        } 
        filterByVarience(record, iteration, res, &count);     
        fclose(fp);  
        free(temp);         
    }  
    */
}

int main(int argc, const char * argv[])
{
    file_cache();
    //createfile(pow(2, 29));
    //file_sread();
    //file_rread();
    //contention();
}

