CC=gcc
CFLAGS=-c

all: coordinator agent main

coordinator:
	$(CC) $(CFLAGS) coordinator.c -o coordinator.o
	ar rcs libcoordinator.a coordinator.o

agent:
	$(CC) $(CFLAGS) agent.c -o agent.o
	ar rcs libagent.a agent.o

main:
	$(CC) main.c -L. -lcoordinator -lagent -o koordynator

clean:
	rm -f *.o *.a koordynator
