# client/Makefile

CC = gcc
CFLAGS = -Wall -Wextra -I../common

all: client2

client2: client2.o ../common/game_logic.o
	$(CC) -o client2 client2.o ../common/game_logic.o

client2.o: client2.c
	$(CC) $(CFLAGS) -c client2.c

../common/game_logic.o:
	$(CC) $(CFLAGS) -c ../common/game_logic.c -o ../common/game_logic.o

clean:
	rm -f *.o client2
	rm -f ../common/*.o
