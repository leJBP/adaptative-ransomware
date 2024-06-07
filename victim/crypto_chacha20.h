
#ifndef CRYPTO_CHACHA_H
#define CRYPTO_CHACHA_H

#include <openssl/evp.h>

#include "files_finder.h"

/**
 * @brief Encrypt a file with chacha20 algorithm 256 bits.
 * @param p_listFileData List of files to encrypt.
 * @param p_key Key to encrypt files.
*/
void chacha20_encrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

/**
 * @brief Decrypt a file with chacha20 algorithm 256 bits.
 * @param p_listFileData List of files to decrypt.
 * @param p_key Key to decrypt files.
*/
void chacha20_decrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

/**
 * @brief Load the encryption key.
 * @param p_key Key to encrypt files.
 * @param p_nonce Initialization vector.
 * @return The encryption key structure.
*/
EVP_CIPHER_CTX* load_encryption_key(unsigned char* p_key, unsigned char* p_nonce);

/**
 * @brief Load the decryption key.
 * @param p_key Key to decrypt files.
 * @param p_nonce Initialization vector.
 * @return The decryption key structure.
*/
EVP_CIPHER_CTX* load_decryption_key(unsigned char* p_key, unsigned char* p_nonce);

#endif