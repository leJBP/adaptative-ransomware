# Définition des Variables (à compléter)
CC=gcc
CFLAGS=-Wall -pedantic 
LDFLAGS= 

# Règles (à compléter avec des variables automatiques)
all: test clean

test: files_finder.o test.o
	$(CC) $(CFLAGS) $^ -o $@

makedepend:
	makedepend *.c -Y

clean:
	rm *.o

.PHONY: clean all makedepend

# DO NOT DELETE

files_finder.o: files_finder.h
test.o: files_finder.h