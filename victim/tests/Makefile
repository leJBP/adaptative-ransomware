# Définition des Variables (à compléter)
CC=gcc
CFLAGS=-Wall -pedantic -g
LDFLAGS= -lssl -lcrypto

# Règles (à compléter avec des variables automatiques)
all: test_rsa clean

test_rsa: ../files_finder.o ../crypto_rsa.o ../server.o ../benchmark.o test_rsa.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

makedepend:
	makedepend *.c -Y

clean:
	rm *.o

.PHONY: clean all makedepend

# DO NOT DELETE

../files_finder.o: ../files_finder.h
../crypto_rsa.o: ../crypto_rsa.h ../files_finder.h
../crypto_aes.o: ../crypto_aes.h ../files_finder.h
../server.o: ../server.h ../benchmark.h
../benchmark.o: ../benchmark.h
test_rsa.o: ../files_finder.h ../server.h ../benchmark.h ../crypto_rsa.h