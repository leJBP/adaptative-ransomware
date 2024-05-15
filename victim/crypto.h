#ifndef CRYPTO_H
#define CRYPTO_H

#include "files_finder.h"

#define BUF_SIZE    1024

/**
 * @brief Encrypts a file using a key with RSA algorithm
 * @param p_path Path to the file to encrypt
 * @param p_name Name of the file to encrypt
 * @param p_pubKey public key to use for encryption
*/
void encrypt_files(listFileData* p_listFileData, RSA* p_pubKey);

/**
 * @brief Decrypts a file using a key with RSA algorithm
 * @param p_path Path to the file to decrypt
 * @param p_name Name of the file to decrypt
 * @param p_privKey private key to use for decryption
*/
void decrypt_files(listFileData* p_listFileData, RSA* p_privKey);

#endif