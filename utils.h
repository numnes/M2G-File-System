
#ifndef M2G_UTILS
#define M2G_UTILS

#define POINTERS_PER_INODE 8

#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "bitmask.h"

unsigned long get_size (FILE *stream) {
    unsigned long size;
    fseek(stream, 0L, SEEK_END);
    size = (unsigned long) ftell(stream);
    rewind(stream);
    return size;
}

bool are_free_blocks(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    if(sb.num_free_blocks > 0)
        return true;
    return false;
}

bool are_free_inodes(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    if(sb.num_free_inodes > 0)
        return true;
    return false;
}

unsigned int num_blocks(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    return sb.num_blocks;
}

unsigned int num_inodes(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    return sb.num_inodes;
}

void dec_free_blocks(FILE *device){
    superblock sb;
    sb = read_superblock(device);

    sb.num_free_blocks = sb.num_free_blocks -1;

    fseek(device, 0, SEEK_SET);
    fwrite(&sb, sizeof(superblock), 1, device);
}

void dec_free_inodes(FILE *device){
    superblock sb;
    sb = read_superblock(device);

    sb.num_free_inodes = sb.num_free_inodes -1;

    fseek(device, 0, SEEK_SET);
    fwrite(&sb, sizeof(superblock), 1, device);
}


unsigned int get_empty_block(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_block_bitmp = sb.ad_block_bitmp * block_size;
    unsigned int num_blocks = sb.num_blocks;
    unsigned int index_free_block = 49;

    unsigned char bitmap[num_blocks];

    fseek(device, ad_block_bitmp, SEEK_SET);
    fread(bitmap, sizeof(char) * num_blocks, 1, device);

    for(unsigned int i = 0; i < num_blocks; i++){
        if(!return_bit_at(bitmap, i)){
            index_free_block =  i;
            break;
        }
    }
    return index_free_block;
}

bool set_block_used(FILE *device, unsigned int block){
    superblock sb;
    sb = read_superblock(device);
    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned long ad_block_bitmp = sb.ad_block_bitmp * block_size;
    unsigned int num_blocks = sb.num_blocks;

    unsigned char bitmap[num_blocks];

    fseek(device, ad_block_bitmp, SEEK_SET);
    fread(bitmap, sizeof(unsigned char) * num_blocks, 1, device);

    toggle_bit_at(bitmap, block, true);

    fseek(device, ad_block_bitmp, SEEK_SET);
    fwrite(bitmap, sizeof(unsigned char) * num_blocks, 1, device);

    dec_free_blocks(device);
    
    return true;
}

unsigned int empty_inode(FILE *device){
    superblock sb;
    sb = read_superblock(device);
    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int num_inodes = sb.num_inodes;
    unsigned long ad_inode_tab = sb.ad_inode_tab * block_size;
    unsigned int index_inode_free = 49;
    inode inode_table[num_inodes];

    fseek(device, ad_inode_tab, SEEK_SET);
    fread(inode_table, sizeof(inode) * num_inodes, 1, device);

    for(unsigned int i = 0; i < num_inodes; i++){
        inode inode_at = inode_table[i];
        unsigned char type_inode_at = inode_at.type[0];
        if(type_inode_at == 0){
            index_inode_free = i;
            break;
        }    
    }
    return index_inode_free;
}

std::string get_name_dir(const char *dir_path){
    std::string destiny_path = dir_path;
    if(destiny_path == "")
        return 0;

    std::string delimiter = "/";

    size_t pos = 0;
    std::string token;
    
    while ((pos = destiny_path.find(delimiter)) != std::string::npos) {
        token = destiny_path.substr(0, pos);
        destiny_path.erase(0, pos + delimiter.length());
    }
    return destiny_path;
}

void remove_final_dir(char *path){
    int size= strlen(path);
    int index_last_separator = 0;
    for(int i = 0; i < size; i++){
        if(path[i] == '/')
            index_last_separator = i;
    }
    if(index_last_separator == 0)
        return;
    char temp_str[index_last_separator];
    for(int i = 0; i < index_last_separator; i++){
        temp_str[i] = path[i];
    }
    memset(path, 0, size);
    strcpy(path, temp_str);
    path[index_last_separator] = '\0';
}

#endif