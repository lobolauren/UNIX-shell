CFLAGS = -Wpedantic -std=gnu99
CC = gcc

all:  myShell

myShell: myShell.o 
	$(CC) $(CFLAGS) myShell.c -o myShell

clean:
		rm *.o
