# Définition des Variables (à compléter)
CC=gcc
CFLAGS=-Wall -pedantic -g
LDFLAGS= -lssl -lcrypto -fopenmp

# Règles (à compléter avec des variables automatiques)
all: ransomware clean

ransomware: files_finder.o crypto_rsa.o ransomware.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: files_finder.o crypto_chacha20.o test.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

makedepend:
	makedepend *.c -Y

clean:
	rm *.o

.PHONY: clean all makedepend

# DO NOT DELETE

files_finder.o: files_finder.h
ransomware.o: files_finder.h
crypto_rsa.o: crypto_rsa.h files_finder.h
crypto_aes.o: crypto_aes.h files_finder.h
crypto_chacha20.o: crypto_chacha20.h files_finder.h
server.o: server.h
test.o: files_finder.h crypto_chacha20.h
benchmark.o: benchmark.h