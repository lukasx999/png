CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb
DEPS=png.h
LIBS=-lz

all: main.o png.o $(DEPS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o out

%.o: %.c Makefile $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o out

.PHONY: clean
