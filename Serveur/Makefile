# server/Makefile

CC = gcc
CFLAGS = -Wall -Wextra -I../common

all: server2

server2: server2.o ../common/game_logic.o
	$(CC) -o server2 server2.o ../common/game_logic.o

server2.o: server2.c
	$(CC) $(CFLAGS) -c server2.c

../common/game_logic.o:
	$(CC) $(CFLAGS) -c ../common/game_logic.c -o ../common/game_logic.o

clean:
	rm -f *.o server2
	rm -f ../common/*.o
