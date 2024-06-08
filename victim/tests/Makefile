# Définition des Variables (à compléter)
CC=gcc
CFLAGS=-Wall -pedantic -g
LDFLAGS= -lssl -lcrypto

# Règles (à compléter avec des variables automatiques)
all: test_chacha20 test_connection test_get_identifier clean

test_chacha20: ../files_finder.o ../crypto_rsa.o test_chacha20.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_aes: ../files_finder.o ../crypto_aes.o test_aes.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_rsa: ../files_finder.o ../crypto_rsa.o test_rsa.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_connection: ../files_finder.o ../server.o ../benchmark.o test_connection.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_get_identifier: ../benchmark.o test_get_identifier.o
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
test_chacha20.o: ../files_finder.h ../crypto_chacha20.h
test_aes.o: ../files_finder.h ../crypto_aes.h
test_rsa.o: ../files_finder.h ../crypto_rsa.h
test_connection.o: ../files_finder.h ../server.h ../benchmark.h
test_get_identifier.o: ../benchmark.h