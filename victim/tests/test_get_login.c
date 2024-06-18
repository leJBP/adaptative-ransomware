#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../files_finder.h"

int main() {
    char* p_login = get_username();
    char* pp_targetFolders[] = {"Downloads/", "Desktop/", "Music/", "Pictures/", "Videos/"};
    char* p_pathBase = NULL;
    char** pp_paths = NULL;

    p_pathBase = malloc(strlen("/home/") + strlen(p_login) + 2);
    if (p_pathBase == NULL) {
        perror("[-] malloc failed");
        exit(1);
    }

    sprintf(p_pathBase, "/home/%s/", p_login);

    // Allouer de la mémoire pour le tableau de chemins
    pp_paths = malloc(sizeof(char*) * (sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0])));
    if (pp_paths == NULL) {
        perror("[-] malloc failed");
        exit(1);
    }

    // Construire les chemins et les stocker dans le tableau
    for (int i = 0; i < sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0]); i++) {
        pp_paths[i] = malloc(strlen(p_pathBase) + strlen(pp_targetFolders[i]) + 1);
        if (pp_paths[i] == NULL) {
            perror("[-] malloc failed");
            exit(1);
        }
        sprintf(pp_paths[i], "%s%s", p_pathBase, pp_targetFolders[i]);
    }

    // Afficher les chemins
    for (int i = 0; i < sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0]); i++) {
        printf("%s\n", pp_paths[i]);
    }

    /* Analyse des fichiers */
    listFileData* p_listFileData = init_list_file_data();

    files_finder(p_listFileData, pp_paths, sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0]));

    print_path_data(p_listFileData);

    // Nettoyer la mémoire
    free(p_pathBase);
    for (int i = 0; i < sizeof(pp_targetFolders) / sizeof(pp_targetFolders[0]); i++) {
        free(pp_paths[i]);
    }
    free_path_data(p_listFileData);
    free(pp_paths);

    return 0;
}
