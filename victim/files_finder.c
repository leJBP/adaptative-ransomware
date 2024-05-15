#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "files_finder.h"

static void add_file(listFileData* p_listFileData, char* p_name, char* p_path)
{
    fileData* p_newFileData = malloc(sizeof(fileData));
    /* Error detection in memory allocation */
    if (p_newFileData == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p_newFileData->p_name = p_name;
    p_newFileData->p_path = p_path;
    p_newFileData->p_next = p_listFileData->p_head; // Linking to the previous head

    /* New head */
    p_listFileData->p_head = p_newFileData;
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
            //printf("%s\n", dp->d_name);

            /* If it's a file we add it in our files data structure */
            if (dp->d_type == 8)
            {
                add_file(p_listFileData, dp->d_name, p_path);
            }

            // Construct new path from our base path
            strcpy(updatedPath, p_path);
            strcat(updatedPath, "/");
            strcat(updatedPath, dp->d_name);

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
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p_listFileData->p_head = NULL;
    return p_listFileData;
}

char* get_username()
{
    char* p_username = getlogin();
    if (p_username == NULL)
    {
        fprintf(stderr, "Error: getlogin failed\n");
        exit(EXIT_FAILURE);
    }
    return p_username;
}

void files_finder(listFileData* p_listFileData, char** pp_paths, int countPaths)
{
    for (int i = 0; i < countPaths; i++)
    {
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
        free(p_currentFileData);
        p_currentFileData = p_nextFileData;
    }
    free(p_listFileData);

}

void print_path_data(listFileData* p_listFileData)
{
    fileData* p_currentFileData = p_listFileData->p_head;
    while (p_currentFileData != NULL)
    {
        printf("%s/%s\n", p_currentFileData->p_path, p_currentFileData->p_name);
        p_currentFileData = p_currentFileData->p_next;
    }

}