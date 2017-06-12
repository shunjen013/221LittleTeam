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
    //int fp = open("myfile", O_RDWR|O_TRUNC|O_CREAT|O_DIRECT|O_SYNC);
    int fp = open("/mnt/nfs/home/myfile", O_RDWR|O_TRUNC|O_CREAT|O_DIRECT|O_SYNC);
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

void file_sread() {
    /*
    FILE *f = fopen("myfile", "w");
    unsigned long long int size = pow(2, 32);
    fflush(f);
    ftruncate(fileno(f), size);
    fclose(f);  
    */ 
    unsigned long long int block_num = 1;
    unsigned long long int s = 0;
    unsigned long long int time1, time2;
    int iteration = 5;
    unsigned long long int* record = new unsigned long long int[iteration];
    unsigned long long int* res    = new unsigned long long int[iteration];
    int it=0, count=0;
    int b_s = 4096;
    /*
    for(int i=0; i<20; i++) {
        for(it=0; it<iteration; ++it) {
            int fp = open("/home/yen-ting/data_disk/myfile", O_DIRECT|O_RDONLY|O_SYNC);
            //lseek64(fp, 0, SEEK_SET);
            s = 0;
            block_num = pow(2, i);
            char* buffer = (char*) memalign(b_s, b_s*block_num);
            start(&time1);
            while(s < block_num*b_s) {
                ssize_t gap = read(fp, buffer+s, b_s);
                s += gap;
                //printf("%d \n", gap);
            }
            end(&time2);
            close(fp);
            free(buffer);
            int fd;
            char* data = "3";
            sync();
            fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
            write(fd, data, sizeof(char));
            close(fd);
            record[it] = (time2-time1) / block_num;
        }
        printf("%d \n", i);
        filterByVarience(record, iteration, res, &count);
    }
    */
    
    for(int i=8; i<18; i++) {
        createfile(pow(2, i) * b_s);
        //int fp = open("/home/yen-ting/data_disk/myfile", O_DIRECT|O_RDONLY);
        int fp = open("/mnt/nfs/home/myfile", O_DIRECT|O_RDWR|O_SYNC);
        //lseek64(fp, 0, SEEK_SET);
        
        s = 0;
        block_num = int(pow(2, i));
        char* buffer = (char*) memalign(b_s, b_s);
        start(&time1);
        int total = 0;
        unsigned long long int t_s = 0;
        for(total=0; total<100; total++) {
            ssize_t gap = read(fp, buffer, b_s);
            s += gap;
            t_s += gap;
            if(s >= block_num * b_s) s = 0;
            //printf("%d \n", gap);
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
    printf("\n \n");
}

void file_rread() {
    /*
    FILE *f = fopen("myfile", "w");
    unsigned long long int size = pow(2, 32);
    fflush(f);
    ftruncate(fileno(f), size);
    fclose(f); 
    */  
    unsigned long long int block_num = 1;
    unsigned long long int s = 0;
    unsigned long long int time1, time2;
    int iteration = 10;
    unsigned long long int* record = new unsigned long long int[iteration];
    unsigned long long int* res    = new unsigned long long int[iteration];
    int it=0, count=0;
    int b_s = 4096;
    srand(time(NULL));
    /*
    for(int i=0; i<20; i++) {
        for(it=0; it<iteration; ++it) {
            int fp = open("myfile", O_DIRECT|O_RDONLY|O_SYNC);
            //lseek64(fp, 0, SEEK_SET);
            s = 0;
            block_num = pow(2, i);
            char* buffer = (char*) memalign(b_s, b_s*block_num);
            start(&time1);
            while(s < block_num*b_s) {
                ssize_t gap = read(fp, buffer+s, b_s);
                s += gap;
                lseek64(fp, rand()%block_num*b_s, SEEK_SET);
                //printf("%d \n", gap);
            }
            end(&time2);
            close(fp);
            free(buffer);
            sync();
            int fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
            write(fd, "3", 1);
            close(fd);
            record[it] = (time2-time1) / block_num;
        }
        printf("%d \n", i);
        filterByVarience(record, iteration, res, &count);
    }
    */
    for(int i=8; i<18; i++) {
        createfile(pow(2, i) * b_s);
        //int fp = open("/home/yen-ting/data_disk/myfile", O_DIRECT|O_RDONLY);
        //int fp = open("myfile", O_DIRECT|O_RDONLY);
        int fp = open("/mnt/nfs/home/myfile", O_DIRECT|O_RDONLY|O_SYNC);
        //lseek64(fp, 0, SEEK_SET);
        s = 0;
        block_num = pow(2, i);
        char* buffer = (char*) memalign(b_s, b_s);
        start(&time1);
        int total = 0;
        unsigned long long int t_s = 0;
        for(total=0; total<100; total++) {
            lseek64(fp, rand()%block_num*b_s, SEEK_SET);
            ssize_t gap = read(fp, buffer, b_s);
            s += gap;
            t_s += gap;
            if(s >= block_num * b_s) s = 0;
        }
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
    
}

void contention() {
    
}

int main(int argc, const char * argv[])
{
    //file_cache();
    //createfile(pow(2, 30));
    file_sread();
    file_rread();
    //contention();
}

