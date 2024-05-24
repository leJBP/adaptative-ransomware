#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"
#include "files_finder.h"

int main(int argc, char const *argv[])
{
    /* code */

    char* paths[] = {"/tmp/sandbox-ransomware/"};
    /* Initialisation de la structure pour la recherche de fichier */
    listFileData* p_listFileData = init_list_file_data();

    /* Indexation des fichiers à chiffrer */
    files_finder(p_listFileData, paths, 1);

    /* Affichage des fichiers indexés */
    print_path_data(p_listFileData);

    /* Benchmark */
    benchmarkData* p_data = benchmark_pc(p_listFileData->totalSize);

    /* Print benchmark data */
    printf("Memory size: %d\n", p_data->memorySize);
    printf("CPU max freq: %f\n", p_data->cpuMaxFreq);
    printf("CPU min freq: %f\n", p_data->cpuMinFreq);
    printf("CPU core: %d\n", p_data->cpuCore);
    printf("Data size: %d\n", p_data->dataSize);

    free_path_data(p_listFileData);
    free(p_data);

    return 0;
}
