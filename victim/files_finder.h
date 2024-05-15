#ifndef FILES_FINDER_H
#define FILES_FINDER_H

struct pathData
{
    char* p_path;
    struct fileData* p_file;
    struct pathData* p_next;
};
typedef struct pathData pathData;

struct fileData
{
    char* p_name;
    struct fileData* p_next;
};
typedef struct fileData fileData;

/**
 * @brief Initialize a pathData struct.
 * @return pathData* an initialized pathData struct
*/
pathData* init_path_data();

/**
 * @brief Get the username of the current user.
 * @return char* the username of the current user
*/
char* get_username();

/**
 * @brief launch the findind files process, at the end the pathData struct is filled with the files found.
 * @warning the pp_pathss must only contains path which not overlap
 * @param p_pathData the pathData struct to fill
 * @param pp_paths the paths to search in
 * @param countPaths the number of paths to search in
 * @return pathData* the pathData struct filled with the files found
*/
pathData* files_finder(pathData* p_pathData, char** pp_paths, int countPaths);

/**
 * @brief Delete the pathData struct.
 * @param path_data the pathData struct to delete
*/
void free_path_data(pathData* p_pathData);

/**
 * @brief Print the pathData struct.
 * @param path_data the pathData struct to print
*/
void print_path_data(pathData* p_pathData);

#endif
