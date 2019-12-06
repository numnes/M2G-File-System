#ifndef M2G_READER
#define M2G_READER

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "utils.h"

superblock read_superblock(FILE *device){
    superblock sb;
    fseek(device, 0, SEEK_SET);
    fread(&sb, sizeof(superblock), 1, device);
    rewind(device);
    return sb;
}

inode get_inode_by_index(FILE *device, unsigned int index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int inode_address = (sb.ad_inode_tab * block_size) + (index * 62);

    fseek(device, inode_address, SEEK_SET);

    inode inode_at;
    fread(&inode_at, sizeof(inode), 1, device);

    return inode_at;
}

std::vector<std::pair<unsigned int, std::string> > get_dir_entries_from_inode(FILE *device, unsigned int index_inode){
    superblock sb;
    sb = read_superblock(device);
    
    std::vector<std::pair<unsigned int, std::string> > dir_entries;

    unsigned int block_size = (1024 << sb.pot_block_size);
    inode inode_at = get_inode_by_index(device, index_inode);
    unsigned int npointers = ceil(inode_at.size / block_size);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;

    char free_dir_name[27];
    memset(free_dir_name, 0, 27);

    for(int i = 0; i < 8; i++){
        if(inode_at.direct_pointers[i] != null_ptr_dir){
            unsigned long pointer = (inode_at.direct_pointers[i]) * block_size;
            fseek(device, pointer, SEEK_SET);
            for( int j = 0; j < block_size; j += sizeof(directory_entry) ){
                directory_entry temp_de;
                fread(&temp_de, sizeof(directory_entry), 1, device);
                if(temp_de._type == 1 || temp_de._type == 2 || temp_de._type == 3){
                    std::string name = temp_de.name;
                    unsigned int index_inode = temp_de.index_inode;
                    dir_entries.push_back({index_inode, name});
                }
            }
        }else{
            return dir_entries;
        }
    }
    return dir_entries;
    if(inode_at.inderect_pointer != null_ptr_indir){
        std::vector<std::pair<unsigned int, std::string> > indirect_entries;
        indirect_entries = get_dir_entries_from_inode(device, inode_at.inderect_pointer);
        
        for(int i = 0; i < indirect_entries.size(); i++){
            dir_entries.push_back(indirect_entries[i]);
        }
    }
    return dir_entries;
}

std::vector<directory_entry> get_dir_entries_from_inode_object(FILE *device, unsigned int index_inode){
    superblock sb;
    sb = read_superblock(device);
    
    std::vector<directory_entry> dir_entries;

    unsigned int block_size = (1024 << sb.pot_block_size);
    inode inode_at = get_inode_by_index(device, index_inode);
    unsigned int npointers = ceil(inode_at.size / block_size);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;

    char free_dir_name[27];
    memset(free_dir_name, 0, 27);

    for(int i = 0; i < 8; i++){
        if(inode_at.direct_pointers[i] != null_ptr_dir){
            unsigned long pointer = (inode_at.direct_pointers[i]) * block_size;
            fseek(device, pointer, SEEK_SET);
            for( int j = 0; j < block_size; j += sizeof(directory_entry) ){
                directory_entry temp_de;
                fread(&temp_de, sizeof(directory_entry), 1, device);
                if(temp_de._type == 1 || temp_de._type == 2 || temp_de._type == 3){
                    dir_entries.push_back(temp_de);
                }
            }
        }else{
            return dir_entries;
        }
    }
    return dir_entries;
    if(inode_at.inderect_pointer != null_ptr_indir){
        std::vector<directory_entry> indirect_entries;
        indirect_entries = get_dir_entries_from_inode_object(device, inode_at.inderect_pointer);
        
        for(int i = 0; i < indirect_entries.size(); i++){
            dir_entries.push_back(indirect_entries[i]);
        }
    }
    return dir_entries;
}


unsigned int find_dir_entry(FILE *device, std::string name, unsigned int index_inode){
    std::vector<std::pair<unsigned int, std::string> > entries = get_dir_entries_from_inode(device, index_inode);

    for(int i = 0; i < entries.size(); i++){
        if(entries[i].second == name){
            return entries[i].first;
        }
    }
    return UINT_MAX;
}

unsigned int find_inode_from_path(FILE *device, const char *path){
    superblock sb;
    sb = read_superblock(device);

    std::string destiny_path = path;
    if(destiny_path == "")
        return 0;

    std::vector<std::string> directories_path;
    std::string delimiter = "/";

    size_t pos = 0;
    std::string token;
    
    while ((pos = destiny_path.find(delimiter)) != std::string::npos) {
        token = destiny_path.substr(0, pos);
        directories_path.push_back(token);
        destiny_path.erase(0, pos + delimiter.length());
    }
    directories_path.push_back(destiny_path);

    std::string at_dir = directories_path[0];
    unsigned int inode_index = find_dir_entry(device, at_dir, 0);

    for(int i = 1; i < directories_path.size(); i++){
        at_dir = directories_path[i];
        inode_index = find_dir_entry(device, at_dir, inode_index);
        if(inode_index == UINT_MAX)
            break;
    }
    return inode_index;
}

#endif