#ifndef M2G_WRITTER_VIEW_MODE
#define M2G_WRITTER_VIEW_MODE

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

#include "structures.h"
#include "utils.h"
#include "writer.h"
#include "reader.h"

void print_date_from_epoch(time_t time){
    struct tm  ts;
    char buf[80];
    ts = *localtime(&time);
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &ts);
    printf("%s", buf);
}

std::vector<directory_entry> print_folder_content(FILE *device, unsigned int inode_index, std::string folder_name = "root"){
    std::string tipes[4] = {"FREE", "file", "directory", "link"};
    std::vector<directory_entry> entries = get_dir_entries_from_inode_object(device, inode_index);

    std::cout << std::endl << "Tamanho\t\tCriado Em\t\tModificado Em\t\tTipo\t\tNome" << std::endl;
    for(int i = 0; i < entries.size(); i++){
        inode inode_at = get_inode_by_index(device, entries[i].index_inode);

        printf("%5lubytes\t", inode_at.size);

        print_date_from_epoch(inode_at.creation_time);
        std::cout << "\t";
        print_date_from_epoch(inode_at.modified_time);

        char tp_v[tipes[entries[i]._type].size()];
        for(int j = 0; j < tipes[entries[i]._type].size(); j++)
            tp_v[j] = tipes[entries[i]._type][j];
        tp_v[tipes[entries[i]._type].size()] = '\0';
        printf("\t%13s", tp_v);
        std::cout << "\t" << entries[i].name << std::endl;
    }
    return entries;
}

int get_idex_list_de_by_name(std::string name, std::vector<directory_entry> entries){
    for(int i = 0; i < entries.size(); i++){
        if(entries[i].name == name)
            return i;
    }
    return INT_MAX;
}

std::string get_entry_command(std::string command){
    int ncoms = 0;

    std::string destiny_path = command;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;

    while ((pos = destiny_path.find(delimiter)) != std::string::npos) {
        token = destiny_path.substr(0, pos);
        ncoms ++;
        destiny_path.erase(0, pos + delimiter.length());
    }
    ncoms ++;
    if(ncoms > 2)
        destiny_path = "ERROR NAME";
    return destiny_path;
}

void view_mode(const char *device_name){
    FILE *device;
    if(!(device = fopen(device_name, "rb+"))){
        std::cout << "ERRO!\n\nNão foi possivel acessar o disco!" << std::endl;
        return;
    }
    std::string str_device_name = device_name;
    std::vector<directory_entry> entries;
    entries = print_folder_content(device, 0, str_device_name);
    std::string command;

    std::string current_folder = str_device_name;
    int count_folder = 0;

    while(1){
        std::cout << std::endl << ":[" << current_folder << "]$ cd ";
        std::cin >> command;
        if(command == "exit")
            break;

        unsigned int index = UINT_MAX;
        std::string directorie_cd_name = get_entry_command(command);

        for(unsigned int i = 0; i < entries.size(); i++){
            std::string st_name = entries[i].name;
            if(st_name == directorie_cd_name){
                if(entries[i]._type != 2){
                    std::cout << "A entrada não é um diretorio!" << std::endl;
                    index = UINT_MAX;
                }
                else{
                    index =  i;
                }
            }
        }
        if(index == UINT_MAX)
            continue;

        if(!strcmp(entries[index].name, "..")){
            std::string temp_name = current_folder;
            int ind;
            for(int i = 0; i < temp_name.size(); i++)
                if(temp_name[i] == '/')
                    ind = i;
            temp_name = "";
            for(int j = 0; j < ind; j++)
                temp_name += current_folder[j];
            current_folder = temp_name;
        }
        else{
            current_folder += "/";
            current_folder += entries[index].name;
        }
        entries = print_folder_content(device, entries[index].index_inode, current_folder);
    }
    fclose(device);
}

#endif
