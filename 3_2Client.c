#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<iostream>

<<<<<<< HEAD
#define PORT "8888"
#define IP "10.42.0.1"
=======
#define PORT "5566"
#define IP "127.0.0.1"
>>>>>>> 756aada493eb332a5bdb0cf732e5507b1bfdea93

#define ITERATION 100
#define ARR_SIZE 10000
#define TOTAL 160000000

using namespace std;


int main(int argv, char * argc [])
{
	/*
		1. getaddrinfo
		2. create a new socket
		3. connect to the socket
		4. send data
	*/	
	
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

	
	status = connect(socket_id, res->ai_addr, res->ai_addrlen);
	if(status < 0)
	{
		fprintf(stderr, "Error connect \n");
		exit(3);
	}
	
	int numbytes = 0;
	char* buf = new char[ARR_SIZE];
	char* msg = new char[ARR_SIZE];
	for (int i = 0; i < ARR_SIZE; i ++) msg[i] = 'a';

	cerr << "Start" << endl;
	for (int i = 0; i < ITERATION; i ++) {
		unsigned long long int numbytes = 0;
		while (numbytes < TOTAL) { 
			numbytes += recv(socket_id, msg, ARR_SIZE, 0);
		}
	}
	cerr << "End" << endl;


	delete [] buf;
	delete [] msg;
	freeaddrinfo(res);
	close(socket_id);	
	return 0;
}
