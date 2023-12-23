CC = gcc
CFLAGS = -Wall

buildrun:
	@$(CC) $(CFLAGS) main.c -o server
	@./server
