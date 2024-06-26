#ifndef CRYPTO_RSA_H
#define CRYPTO_RSA_H

#include <openssl/evp.h>

#include "files_finder.h"

#define BUF_SIZE 1024
#define PADDING_SIZE 11

/**
 * @brief Encrypts a file using a key with RSA algorithm
 * @param p_listFileData List of files to encrypt
 * @param p_pubKey public key to use for encryption
*/
void rsa_encrypt_files(listFileData* p_listFileData, EVP_PKEY* p_pubKey);

/**
 * @brief Decrypts a file using a key with RSA algorithm
 * @param p_listFileData List of files to decrypt
 * @param p_privKey private key to use for decryption
*/
void rsa_decrypt_files(listFileData* p_listFileData, EVP_PKEY* p_privKey);

/**
 * @brief Load a key from a file
 * @param p_filename Path to the file containing the key
 * @param selection OSSL_KEYMGMT_SELECT_PUBLIC_KEY if the key is a public key, OSSL_KEYMGMT_SELECT_PRIVATE_KEY if it is a private key
 * @return The key loaded
*/
EVP_PKEY* rsa_load_key(char* p_filename, int selection);

#endif