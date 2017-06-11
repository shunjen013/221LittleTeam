#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<iostream>

#include <stdlib.h>
#include <sys/types.h>
#include "utility.h"
#include <time.h>
#include <sys/time.h>
#include <asm/unistd.h>

#define PORT "8888"

#define ITERATION 100
#define ARR_SIZE 10000
#define TOTAL 160000000

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


void * get_in_addr(struct sockaddr * sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr); 
	}
	
	return &(((struct sockaddr_in6 *)sa)->sin6_addr); 
}

int main(int argc, char * argv[])
{
	// for time measurement
	unsigned long long int time1, time2;
	unsigned long long int* record = new unsigned long long int[ITERATION];
	unsigned long long int* result = new unsigned long long int[ITERATION];
	unsigned long long int mean, var, ans;
	int count=0, it=0;
	unsigned long long int idx = 0;

	// Variables for writing a server. 
	/*
	1. Getting the address data structure.
	2. Openning a new socket.
	3. Bind to the socket.
	4. Listen to the socket. 
	5. Accept Connection.
	6. Receive Data.
	7. Close Connection. 
	*/
	int status;
	struct addrinfo hints, * res;
	int listner; 
	
	
	// Before using hint you have to make sure that the data structure is empty 
	memset(& hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE; 
	
	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if(status != 0)
	{
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	}
	
	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
	if(listner < 0 )
	{
		fprintf(stderr,"socket error: %s\n",gai_strerror(status));
	}
	
	// solved 
	int yes=1;
	if (setsockopt(listner, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
	    perror("setsockopt");
	    _exit(1);
	}	
	// Bind the socket to the address of my local machine and port number 	
	status = bind(listner, res->ai_addr, res->ai_addrlen); 
	if(status < 0)
	{
		fprintf(stderr,"bind: %s\n",gai_strerror(status));
	}

	status = listen(listner, 10); 
	if(status < 0)
	{
		fprintf(stderr,"listen: %s\n",gai_strerror(status));
	}
	
	// Free the res linked list after we are done with it	
	freeaddrinfo(res);
	
	
	// We should wait now for a connection to accept
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN]; // an empty string 
		
	// Calculate the size of the data structure	
	addr_size = sizeof client_addr;
	
	printf("I am now accepting connections ...\n");
	
	// Accept a new connection and return back the socket desciptor 
	new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);	
	if(new_conn_fd < 0)
	{
		fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
	}
	
	inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s); 
	printf("I am now connected to %s \n",s);


	char* buf = new char[ARR_SIZE];
	char* msg = new char[ARR_SIZE];
	for (int i = 0; i < ARR_SIZE; i ++) msg[i] = 'a';
	cerr << "Start" << endl;

	for (int it = 0; it < ITERATION; it ++) {
		cout << "s" << it << endl;
		unsigned long long int numbytes = 0;
		start (&time1);
		while (numbytes < TOTAL) { 
			numbytes += send(new_conn_fd, msg, ARR_SIZE, 0);
		}	
		end (&time2);
		record[it] = time2 - time1;
		cout << "e" << it << endl;
		//printf("Received %s \n", buf);
	}
	
	// Close the socket before we finish 
	close(new_conn_fd);
	
	cerr << "Finish" << endl;
	ans = filterByVarience(record, ITERATION, result, &count);
	 
	delete [] buf;
	delete [] msg;	
	delete [] record;
	delete [] result;
	
	return 0;
}
