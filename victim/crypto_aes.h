/**
 * The AES implementation is inspired by the following source:
 * https://github.com/saju/misc/blob/master/misc/openssl_aes.c
*/

#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

#include <openssl/evp.h>

#include "files_finder.h"

/**
 * @brief Encrypt a file with AES algorithm 256 bits.
 * @param p_listFileData List of files to encrypt.
 * @param p_key Key to encrypt files.
*/
void aes_encrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

/**
 * @brief Decrypt a file with AES algorithm 256 bits.
 * @param p_listFileData List of files to decrypt.
 * @param p_key Key to decrypt files.
*/
void aes_decrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

/**
 * @brief Load the encryption key.
 * @param p_key Key to encrypt files.
 * @param p_iv Initialization vector.
 * @return The encryption key structure.
*/
EVP_CIPHER_CTX* load_aes_encryption_key(unsigned char* p_key, unsigned char* p_iv);

/**
 * @brief Load the decryption key.
 * @param p_key Key to decrypt files.
 * @param p_iv Initialization vector.
 * @return The decryption key structure.
*/
EVP_CIPHER_CTX* load_aes_decryption_key(unsigned char* p_key, unsigned char* p_iv);

#endif