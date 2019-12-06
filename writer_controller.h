#ifndef M2G_WRITTER_CONTROLLER
#define M2G_WRITTER_CONTROLLER

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "utils.h"
#include "writer.h"
#include "reader.h"

bool import_file(const char *file_source_name, const char *device_name, const char* path_into_device, bool root){
    FILE *file_source;
    FILE *device;
    bool ret = true;

    file_source = fopen(file_source_name,"rb+");
    device = fopen(device_name,"rb+");
    
    unsigned int inode_destiny = 0;
    if(!root){
        inode_destiny = find_inode_from_path(device, path_into_device);
    }
    
    if(inode_destiny == UINT_MAX){
        std::cout << "ERRO!\n\nO caminho para a pasta de destino não foi encontrado!" << std::endl;
        return false;
    }

    std::string name_source = get_name_dir(file_source_name);
    
    int size_vchar = name_source.length();
    char name_vchar[size_vchar];
    for(int i = 0; i < size_vchar; i++)
        name_vchar[i] = name_source[i];
    name_vchar[size_vchar] = '\0';

    int path_new_file_size;
    if(inode_destiny != 0)
        path_new_file_size = strlen(name_vchar) + strlen(path_into_device) + 1;
    else
        path_new_file_size = strlen(name_vchar);

    char str_path_new_file[path_new_file_size];

    if(inode_destiny != 0){
        strcpy(str_path_new_file, path_into_device);
        if(inode_destiny != 0){
            strcat(str_path_new_file, "/");
            strcat(str_path_new_file, name_vchar);
        }
    }
    else{
        strcpy(str_path_new_file, name_vchar);
    }
    
    if(find_inode_from_path(device, str_path_new_file) != UINT_MAX){
        std::cout << "\tJá existe um arquivo/diretorio com este mesmo nome na pasta de destino." << std::endl;
        ret = false;
    }
    else{
        directory_entry dir_entry_at = create_dir_entry(device, 1, name_vchar);
        
        unsigned long data_length = get_size(file_source);
        
        if(!write_data_in_inode_from_file(device, file_source, dir_entry_at.index_inode, &data_length)){
            std::cout << "ERRO!\n\nNão foi possivel gravar o arquivo!" << std::endl;
            //remover o dir entry
            return false;
        }

        if(!write_directory_entry_in_inode(device, dir_entry_at, inode_destiny)){
            std::cout << "ERRO!\n\nNão foi possivel criar o arquivo!" << std::endl;
            return false;
        }

        inode inode_new_file = get_inode_by_index(device, dir_entry_at.index_inode);
        inode inode_folder = get_inode_by_index(device, inode_destiny);
        inode_folder.size += inode_new_file.size;
        inode_update(device, inode_destiny, inode_folder);
        std::cout << inode_new_file.size << " bytes gravados." << std::endl;
    }
    fclose(device);
    fclose(file_source);
    return ret;
}

bool export_file(const char *folder_destiny_name, const char *device_name, const char* path_into_device){
    FILE *device;
    FILE *new_file;

    device = fopen(device_name, "rb+");

    int size_str_path = strlen(folder_destiny_name);
    char path_to_folder[size_str_path];
    strcpy(path_to_folder, folder_destiny_name);

    unsigned int inode_destiny = find_inode_from_path(device, path_into_device);

    if(inode_destiny == UINT_MAX){
        std::cout << "Erro!\nNão foi possivel obter o caminho para o novo diretorio!" << std::endl;
        return false;
    }

    std::string name_source = get_name_dir(path_into_device);
    int size_vchar = name_source.length();

    char name_file_vchar[size_vchar];

    for(int i = 0; i < size_vchar; i++)
        name_file_vchar[i] = name_source[i];
    name_file_vchar[size_vchar] = '\0';

    int catname_size = (strlen(path_to_folder) + strlen(name_file_vchar)) + 1;

    char cat_name[catname_size];

    if(!strcmp(path_to_folder, "/")){
        strcpy(cat_name, name_file_vchar);
    }
    else{
        strcpy(cat_name, path_to_folder);
        if(strcmp(path_to_folder, name_file_vchar)){
            strcat(cat_name, "/");
            strcat(cat_name, name_file_vchar);
        }
    }
    
    new_file = fopen(cat_name, "w+");
    fclose(new_file);

    new_file = fopen(cat_name, "rb+");

    inode in = get_inode_by_index(device, inode_destiny);

    unsigned long data_length = in.size;

    write_in_file_from_inode(device, new_file, inode_destiny, &data_length);

    fclose(device);
    fclose(new_file);
    return true;
}

bool mk_dir(const char *device_name, const char *path_into_device){
    FILE *device;
    bool ret = true;
    device = fopen(device_name, "rb+");
    std::string nameDir = get_name_dir(path_into_device);
    int size_str_path = strlen(path_into_device);
    char path_to_folder[size_str_path];
    strcpy(path_to_folder, path_into_device);

    remove_final_dir(path_to_folder);

    unsigned int inode_destiny = 0;
    if(strcmp(path_to_folder, path_into_device)){
        inode_destiny = find_inode_from_path(device, path_to_folder);
    }

    if(inode_destiny == UINT_MAX){
        std::cout << "Erro!\nNão foi possivel obter o caminho para o novo diretorio!" << std::endl;
        return false;
    }

    std::string name_source = get_name_dir(path_into_device);

    int size_vchar = name_source.length();

    char name_vchar[size_vchar];

    for(int i = 0; i < size_vchar; i++)
        name_vchar[i] = name_source[i];
    name_vchar[size_vchar] = '\0';

    int path_new_file_size = strlen(name_vchar) + strlen(path_into_device) + 1;
    char str_path_new_file[path_new_file_size];
    strcpy(str_path_new_file, path_into_device);
    if(inode_destiny != 0){
        strcat(str_path_new_file, "/");
        strcat(str_path_new_file, name_vchar);
    }

    if(find_inode_from_path(device, str_path_new_file) != UINT_MAX){
        std::cout << "\tJá existe um diretorio/arquivo com este mesmo nome na pasta de destino." << std::endl;
        ret = false;
    }
    else{
        directory_entry dir_entry_at = create_dir_entry(device, 2, name_vchar);

        directory_entry dir_prev;
        dir_prev.index_inode = inode_destiny;
        strcpy(dir_prev.name, "..");
        dir_prev._type = 2;

        write_directory_entry_in_inode(device, dir_prev, dir_entry_at.index_inode);
        if(!write_directory_entry_in_inode(device, dir_entry_at, inode_destiny)){
            std::cout << "ERRO!\n\nNão foi possivel criar a pasta!" << std::endl;
            return false;
        }
    }
    fclose(device);
    return ret;
}

bool rm_file(const char *device_name, const char *path_into_device){
    FILE *device;
    device = fopen(device_name, "rb+");
    std::string nameDir = get_name_dir(path_into_device);
    int size_str_path = strlen(path_into_device);
    char path_to_folder[size_str_path];
    strcpy(path_to_folder, path_into_device);

    remove_final_dir(path_to_folder);

    unsigned int inode_destiny = inode_destiny = find_inode_from_path(device, path_to_folder);;
    if(inode_destiny == UINT_MAX){
        std::cout << "Erro!\nNão foi possivel obter o caminho para o arquivo!" << std::endl;
        return false;
    }
        
    std::string name_source = get_name_dir(path_into_device);

    int size_vchar = name_source.length();

    char name_vchar[size_vchar];
    
    for(int i = 0; i < size_vchar; i++)
        name_vchar[i] = name_source[i];
}


#endif