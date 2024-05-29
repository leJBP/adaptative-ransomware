#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "benchmark.h"

static void get_ram_size(benchmarkData* p_data)
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("[-] read ram size failed on open file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int mem_total = 0;

    /* Read the first line of the file */
    if (fgets(line, sizeof(line), fp) != NULL) {
        /* Check if the line starts with "MemTotal:" */
        if (strncmp(line, "MemTotal:", 8) == 0) {
            /* Extract the value */
            sscanf(line, "MemTotal: %d kB", &mem_total);
        }
    }

    /* Check if the memory has been found */
    if (mem_total == 0) {
        perror("[-] read ram size failed");
        exit(EXIT_FAILURE);
    } else {
        p_data->memorySize = mem_total;
    }

    fclose(fp);

}

static void get_cpu_data(benchmarkData* p_data)
{
    /* Open child process to execute lscpu */
    FILE *fp = popen("lscpu", "r");
    if (fp == NULL) {
        perror("[-] Failed to execute lscpu");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    char* p_maxFreq = NULL;
    char* p_minFreq = NULL;
    char* p_core = NULL;

    /* Read the file line by line */
    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Check if the line contain cpu max info */
        if (strncmp(line, "CPU max MHz:", 11) == 0) {
            p_maxFreq = malloc(sizeof(char) * strlen(strtok(line + 24, " ")) + 1);
            strcpy(p_maxFreq, strtok(line + 24, " "));
        }

        /* Check if the line contain cpu min info */
        if (strncmp(line, "CPU min MHz:", 11) == 0) {
            p_minFreq = malloc(sizeof(char) * strlen(strtok(line + 24, " ")) + 1);
            strcpy(p_minFreq, strtok(line + 24, " "));
            break;
        }

        /* Check if the line contain cpu core info */
        if (strncmp(line, "CPU(s):", 6) == 0) {
            p_core = malloc(sizeof(char) * strlen(strtok(line + 18, " ")) + 1);
            strcpy(p_core, strtok(line + 18, " "));
        }
    }

    /* Check if all the info has been found */
    if (p_maxFreq != NULL && p_minFreq != NULL && p_core != NULL) {
        p_data->cpuMaxFreq = atof(p_maxFreq);
        p_data->cpuMinFreq = atof(p_minFreq);
        p_data->cpuCore = atoi(p_core);
    } else {
        printf("[-] Failed to find CPU max/min frequency or core\n");
    }

    pclose(fp);

    free(p_maxFreq);
    free(p_minFreq);
    free(p_core);
}

benchmarkData* benchmark_pc(int size)
{

    benchmarkData* data = (benchmarkData*)malloc(sizeof(benchmarkData));

    if (data == NULL) {
        perror("[-] benchmark_pc failed");
        exit(EXIT_FAILURE);
    }

    data->dataSize = size;
    get_ram_size(data);
    get_cpu_data(data);

    return data;
}