# Définition des Variables (à compléter)
CC=gcc
CFLAGS=-Wall -pedantic -g
LDFLAGS= -lssl -lcrypto

# Règles (à compléter avec des variables automatiques)
all: ransomware clean

ransomware: files_finder.o crypto.o ransomware.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

makedepend:
	makedepend *.c -Y

clean:
	rm *.o

.PHONY: clean all makedepend

# DO NOT DELETE

files_finder.o: files_finder.h
ransomware.o: files_finder.h
crypto.o: crypto.h files_finder.h