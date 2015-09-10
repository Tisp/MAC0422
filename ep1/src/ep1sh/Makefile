EXEC = ep1sh
CC = gcc
CFLAGS = -Wall -g -O2
LIBS=-lreadline
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)


all: build $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o $(EXEC)

build: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -c $(SRC)

clean:
	rm -f $(OBJ) $(EXEC)


