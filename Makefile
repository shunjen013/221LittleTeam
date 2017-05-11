all: driver123 driver4 driver5 driver2_1
driver123: 123.o utility.o 
	g++ -o driver123 123.o utility.o
driver4: 4.o utility.o
	g++ -pthread -o driver4 4.o utility.o
driver5: 5.o utility.o
	g++ -pthread -o driver5 5.o utility.o
driver2_1: 2_1.o utility.o
	g++ -pthread -o driver2_1 2_1.o utility.o
123.o: 123.c utility.h
	g++ -c 123.c
4.o: 4.c utility.h
	g++ -pthread -c 4.c
5.o: 5.c utility.h
	g++ -pthread -c 5.c
utility.o: utility.c utility.h
	g++ -c utility.c
clean:
	rm -f *.o driver123 driver4 driver5
2_1.o: 2_1.c utility.h
	g++ -pthread -c 2_1.c

