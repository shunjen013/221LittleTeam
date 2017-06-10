all: driver123 driver4 driver5 driver2_1 driver2_2 driver2_3 driver3_1Server driver3_1Client driver3_2Server driver3_2Client driver_f
driver123: 123.o utility.o 
	g++ -o driver123 123.o utility.o
driver4: 4.o utility.o
	g++ -pthread -o driver4 4.o utility.o
driver5: 5.o utility.o
	g++ -pthread -o driver5 5.o utility.o
driver2_1: 2_1.o utility.o
	g++ -O3 -o driver2_1 2_1.o utility.o
driver2_2: 2_2.o utility.o
	g++ -O3 -o driver2_2 2_2.o utility.o
driver2_3: 2_3.o utility.o
	g++ -O3 -o driver2_3 2_3.o utility.o	
driver3_1Server: 3_1Server.o utility.o
	g++ -O3 -o driver3_1Server 3_1Server.o utility.o
driver3_1Client: 3_1Client.o utility.o
	g++ -O3 -o driver3_1Client 3_1Client.o utility.o
driver3_2Server: 3_2Server.o utility.o
	g++ -O3 -o driver3_2Server 3_2Server.o utility.o
driver3_2Client: 3_2Client.o utility.o
	g++ -O3 -o driver3_2Client 3_2Client.o utility.o
driver_f: filesys.o utility.o
	g++ -o driver_f filesys.o utility.o	
123.o: 123.c utility.h
	g++ -c 123.c
4.o: 4.c utility.h
	g++ -pthread -c 4.c
5.o: 5.c utility.h
	g++ -pthread -c 5.c
utility.o: utility.c utility.h
	g++ -c utility.c
clean:
	rm -f *.o driver123 driver4 driver5 driver2_1 driver2_2 driver3_1Server driver3_1Client driver3_2Server driver3_2Client
2_1.o: 2_1.c utility.h
	g++ -c -std=c++11 2_1.c
2_2.o: 2_2.c utility.h
	g++ -c -std=c++11 2_2.c	
2_3.o: 2_3.c utility.h
	g++ -c -std=c++11 2_3.c
3_1Server.o: 3_1Server.c utility.h
	g++ -c 3_1Server.c
3_1Client.o: 3_1Client.c utility.h
	g++ -c 3_1Client.c
3_2Server.o: 3_2Server.c utility.h
	g++ -c 3_2Server.c
3_2Client.o: 3_2Client.c utility.h
	g++ -c 3_2Client.c
filesys.o: filesys.c utility.h
	g++ -c -std=c++11 filesys.c	
