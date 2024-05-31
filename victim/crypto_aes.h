#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

#include <openssl/evp.h>

#include "files_finder.h"


void aes_encrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

void aes_decrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key);

EVP_CIPHER_CTX* load_encryption_key(unsigned char* p_key, unsigned char* p_iv);

EVP_CIPHER_CTX* load_decryption_key(unsigned char* p_key, unsigned char* p_iv);

#endif