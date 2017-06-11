#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>


#include <stdlib.h>
#include <sys/types.h>
#include "utility.h"
#include <time.h>
#include <sys/time.h>

#define ITERATION 100
#define PORT "5566"
#define IP "127.0.0.1"

using namespace std;


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

int main(int argv, char * argc [])
{
	// for time measurement
	unsigned long long int time1, time2;
	unsigned long long int* record_s = new unsigned long long int[ITERATION];
	unsigned long long int* result_s = new unsigned long long int[ITERATION];
	unsigned long long int* record_t = new unsigned long long int[ITERATION];
	unsigned long long int* result_t = new unsigned long long int[ITERATION];
	unsigned long long int mean, var, ans;
	int count=0, it=0;
	unsigned long long int idx = 0;
	/*
		1. getaddrinfo
		2. create a new socket
		3. connect to the socket
		4. send data
	*/
	for (int i = 0; i < ITERATION; i ++) {	
	
		//Variables Declaration
		struct addrinfo hints, * res;
		int status;
		int socket_id;
	
		//clear hints
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		status = getaddrinfo(IP, PORT, &hints, &res);
		if(status != 0)
		{
			fprintf(stderr, "Error getaddrinfo\n");
			exit(1);
		}	 	
	
		socket_id = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(socket_id < 0)
		{
			fprintf(stderr, "Error socket \n");
			exit(2);
		}

		cerr << "start" << i << endl;
		start (&time1);
		status = connect(socket_id, res->ai_addr, res->ai_addrlen);
		end (&time2);
		record_s[i] = time2 - time1;
	
		freeaddrinfo(res);

		start (&time1);
		close(socket_id);
		end (&time2);
		record_t[i] = time2 - time1;

		cerr << "end" << endl;		
	}

	ans = filterByVarience(record_s, ITERATION, result_s, &count);
	ans = filterByVarience(record_t, ITERATION, result_t, &count);

	delete [] record_s;
	delete [] result_s;
	delete [] record_t;
	delete [] result_t;	
	return 0;
}
