CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb
DEPS=png.h

all: main.o png.o $(DEPS)
	$(CC) $(CFLAGS) $^ -o out

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@
