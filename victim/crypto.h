#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>

#include "files_finder.h"

#define BUF_SIZE 1024

/**
 * @brief Encrypts a file using a key with RSA algorithm
 * @param p_path Path to the file to encrypt
 * @param p_name Name of the file to encrypt
 * @param p_pubKey public key to use for encryption
*/
void encrypt_files(listFileData* p_listFileData, EVP_PKEY* p_pubKey);

/**
 * @brief Decrypts a file using a key with RSA algorithm
 * @param p_path Path to the file to decrypt
 * @param p_name Name of the file to decrypt
 * @param p_privKey private key to use for decryption
*/
void decrypt_files(listFileData* p_listFileData, EVP_PKEY* p_privKey);

/**
 * @brief Load a key from a file
 * @param p_filename Path to the file containing the key
 * @param selection 2 if the key is a public key, 1 if it is a private key
 * @return The key loaded
*/
EVP_PKEY* load_key(char* p_filename, int selection );

#endif