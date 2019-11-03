#ifndef M2G_WRITTER_CONTROLLER
#define M2G_WRITTER_CONTROLLER

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "utils.h"
#include "writer.h"
#include "reader.h"

bool import_file(char *file_source_name, char *device_name, char* path_int_device){
    FILE *file_source;
    FILE *device;

    file_source = fopen(file_source_name,"rb+");
    device = fopen(device_name,"rb+");

    unsigned int inode_destiny = find_inode_from_path(device, path_int_device);

    if(inode_destiny == UINT_MAX){
        std::cout << "ERRO!\n\nO caminho para a pasta de destino não foi encontrado!" << std::endl;
        return false;
    }

    std::string name_source = get_name_dir(file_source_name);

    int size_vchar = name_source.length();
    char name_vchar[size_vchar];
    for(int i = 0; i < size_vchar; i++)
        name_vchar[i] = name_source[i];

    directory_entry dir_entry_at = create_dir_entry(device, 1, name_vchar);
    



    return true;
}

#endif