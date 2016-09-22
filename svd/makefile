OBJS = main.o scheduler.o
CC = g++
DEBUG = -g -O3
CFLAGS = -std=c++11 -Wall -fPIC -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

runProg : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o svdSched

main.o : main.cpp scheduler.cpp scheduler.h
	$(CC) $(CFLAGS) main.cpp

cannMiss.o : scheduler.cpp scheduler.h
	$(CC) $(CFLAGS) cannMiss.cpp

clean:	
	\rm *.o svdSched
