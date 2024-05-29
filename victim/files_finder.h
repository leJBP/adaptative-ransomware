#ifndef FILES_FINDER_H
#define FILES_FINDER_H

struct fileData
{
    char* p_path;
    char* p_name;
    struct fileData* p_next;
};
typedef struct fileData fileData;

struct listFileData
{
    fileData* p_head;
    long long totalSize;
};
typedef struct listFileData listFileData;

/**
 * @brief Initialize a listFileData struct.
 * @return listFileData* an initialized listFileData struct
*/
listFileData* init_list_file_data();

/**
 * @brief Get the username of the current user.
 * @return char* the username of the current user
*/
char* get_username();

/**
 * @brief launch the findind files process, at the end the listFileData struct is filled with the files found.
 * @warning the pp_pathss must only contains path which not overlap
 * @param p_listFileData the listFileData struct to fill
 * @param pp_paths the paths to search in
 * @param countPaths the number of paths to search in
 * @return listFileData* the listFileData struct filled with the files found
*/
void files_finder(listFileData* p_listFileData, char** pp_paths, int countPaths);

/**
 * @brief Delete the listFileData struct.
 * @param p_listFileData the listFileData struct to delete
*/
void free_path_data(listFileData* p_listFileData);

/**
 * @brief Print the listFileData struct.
 * @param p_listFileData the listFileData struct to print
*/
void print_path_data(listFileData* p_listFileData);

#endif
