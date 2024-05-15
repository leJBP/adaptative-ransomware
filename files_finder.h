#ifndef FILES_FINDER_H
#define FILES_FINDER_H

struct pathData
{
    char* path;
    struct fileData* file;
    struct pathData* next;
};
typedef struct pathData pathData;

struct fileData
{
    char* name;
    struct fileData* next;
};
typedef struct fileData fileData;

/**
 * @brief Initialize a pathData struct.
 * @return pathData* an initialized pathData struct
*/
pathData* init_path_data();

/**
 * @brief launch the findind files process.
 * @param path_data the pathData struct to fill
 * @return pathData* the pathData struct filled with the files found
*/
pathData* files_finder(pathData* path_data);

/**
 * @brief Delete the pathData struct.
 * @param path_data the pathData struct to delete
*/
void free_path_data(pathData* path_data);

/**
 * @brief Print the pathData struct.
 * @param path_data the pathData struct to print
*/
void print_path_data(pathData* path_data);

#endif
