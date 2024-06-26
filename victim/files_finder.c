#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "files_finder.h"

static void add_file(listFileData* p_listFileData, char* p_name, char* p_path)
{
    fileData* p_newFileData = malloc(sizeof(fileData));
    /* Error detection in memory allocation */
    if (p_newFileData == NULL)
    {
        perror("[-] Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p_newFileData->p_name = p_name;
    p_newFileData->p_path = p_path;
    p_newFileData->p_next = p_listFileData->p_head; // Linking to the previous head
    /* New head */
    p_listFileData->p_head = p_newFileData;

    /* Build file path to get stat */
    struct stat fileStat;
    char* p_filePath = malloc(strlen(p_name) + strlen(p_path) + 1);
    snprintf(p_filePath, strlen(p_name) + strlen(p_path) + 1, "%s%s", p_path, p_name);

    /* Get data about the file */
    if (stat(p_filePath, &fileStat) == -1) 
    {
        perror("[-] get data file stat failed");
        exit(1);
    }

    /* Check if the entry is a file */
    if (S_ISREG(fileStat.st_mode)) 
    {
        /* Add size of the file */
        p_listFileData->totalSize += fileStat.st_size;
    }

    free(p_filePath);

}

/* Inspired from https://codeforwin.org/c-programming/c-program-to-list-all-files-in-a-directory-recursively */
static void analyze_path(listFileData* p_listFileData, char* p_path)
{
    char updatedPath[1000];
    struct dirent *dp;
    /* Open directory stream */
    DIR *dir = opendir(p_path);

    // Unable to open directory stream
    if (!dir)
        return;

    /* Reading of directory */
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            /* If it's a file we add it in our files data structure */
            if (dp->d_type == 8)
            {
                char* p_fileName = malloc(sizeof(char) * (strlen(dp->d_name) + 1));
                if (p_fileName == NULL)
                {
                    perror("[-] Error: malloc failed\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(p_fileName, dp->d_name);
                //printf("%s%s\n", p_path, p_fileName);
                char* p_filePath = malloc(strlen(p_path) + 1);
                if (p_filePath == NULL)
                {
                    perror("[-] Error: malloc failed\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(p_filePath, p_path);
                add_file(p_listFileData, p_fileName, p_filePath);
            }

            // Construct new path from our base path
            strcpy(updatedPath, p_path);
            strcat(updatedPath, dp->d_name);
            strcat(updatedPath, "/");

            analyze_path(p_listFileData, updatedPath);
        }
    }

    /* Closing of directory stream */
    closedir(dir);

}

listFileData* init_list_file_data()
{
    listFileData* p_listFileData = malloc(sizeof(listFileData));
    /* Error detection in memory allocation */
    if (p_listFileData == NULL)
    {
        perror("[-] Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p_listFileData->p_head = NULL;
    return p_listFileData;
}

char* get_username()
{
    char* p_username = getenv("USER");
    if (p_username == NULL)
    {
        perror("[-] Error: getlogin failed\n");
        exit(EXIT_FAILURE);
    }
    return p_username;
}

void files_finder(listFileData* p_listFileData, char** pp_paths, int countPaths)
{
    for (int i = 0; i < countPaths; i++)
    {
        printf("[+] Analyzing path: %s\n", *(pp_paths+i));
        analyze_path(p_listFileData, *(pp_paths+i));
    }
}

void free_path_data(listFileData* p_listFileData)
{
    fileData* p_currentFileData = p_listFileData->p_head;
    fileData* p_nextFileData;
    while (p_currentFileData != NULL)
    {
        p_nextFileData = p_currentFileData->p_next;
        free(p_currentFileData->p_name); // free memory of file name
        free(p_currentFileData->p_path); // free memory of file path
        free(p_currentFileData); // free memory of file data struct
        p_currentFileData = p_nextFileData;
    }
    free(p_listFileData); // free memory of listFileData struct

    printf("[+] Memory freed\n");

}

void print_path_data(listFileData* p_listFileData)
{
    fileData* p_currentFileData = p_listFileData->p_head;
    while (p_currentFileData != NULL)
    {
        printf("%s%s\n", p_currentFileData->p_path, p_currentFileData->p_name);
        p_currentFileData = p_currentFileData->p_next;
    }

}