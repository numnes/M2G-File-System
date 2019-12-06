#ifndef M2G_format_controller
#define M2G_format_controller

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "utils.h"
#include "reader.h"
#include "writer.h"

void clear_reserved_blocks(FILE *device, unsigned int end_reserved){
    unsigned int pointer = end_reserved;
    fseek(device, 0, SEEK_SET);
    while(pointer--){
        fputc(0, device);
    }
}

bool write_superblock(unsigned int pot_block_size, unsigned long device_size, FILE *device){
    superblock sb;
    unsigned int n_inodes;
    unsigned int n_blocks;
    unsigned int num_free_blocks;
    unsigned int block_size;
    unsigned int ad_block_bitmp;
    unsigned char num_blocks_block_bitmp;
    unsigned int ad_inode_table;
    unsigned int ad_data_block;
 
    block_size = (1024 << pot_block_size);

    n_blocks = device_size / block_size;
    n_inodes = n_blocks / POINTERS_PER_INODE;
    ad_block_bitmp = 1;

    double db_n_blocks = n_blocks;
    db_n_blocks /= 8;
    unsigned int n_blocks_bitmap = ceil(db_n_blocks / block_size);
    num_blocks_block_bitmp = (unsigned char)n_blocks_bitmap;

    ad_inode_table = ad_block_bitmp + n_blocks_bitmap;
    double n_inodes_d = (n_inodes * 62) / block_size;
    ad_data_block = ad_inode_table + ceil(n_inodes_d);
    num_free_blocks = n_blocks - ( num_blocks_block_bitmp + 2 );

    sb.num_inodes = n_inodes;
    sb.num_blocks = n_blocks;
    sb.num_free_blocks = num_free_blocks;
    sb.num_free_inodes = n_inodes;
    sb.pot_block_size = pot_block_size;
    sb.ad_block_bitmp = ad_block_bitmp;
    sb.num_blocks_block_bitmp = num_blocks_block_bitmp;
    sb.ad_inode_tab = ad_inode_table;
    sb.ad_data_block = ad_data_block;
    
    clear_reserved_blocks(device, ad_data_block * block_size);
    
    fseek(device, 0, SEEK_SET);
    if(fwrite(&sb, sizeof(superblock), 1, device) != 1)
        return false;

    for(unsigned int i = 0; i < ad_data_block; i++){
        set_block_used(device, i);
    }
 
    return true;
}

bool write_root_inode(FILE *device){
    unsigned int index_inode_root = inode_alloc(device);

    unsigned char data[1] = {0};
    init_root_dir(device, index_inode_root);
    return true;
}

bool hard_format(const char *device_name, unsigned int pot)
{
    FILE *device;
    unsigned long device_size;
    bool flag_success;
    device = fopen (device_name, "rb+");
    
    while(fgetc(device) != EOF){
        fseek(device, -1, SEEK_CUR);
        fputc(0, device);
    }
    
    device_size = get_size(device);

    flag_success = write_superblock(pot, device_size, device);
    flag_success = flag_success && write_root_inode(device);

    fclose(device);
    return flag_success;
}

bool soft_format(const char *device_name, unsigned int pot)
{
    FILE *device;
    unsigned long device_size;
    bool flag_success;
    device = fopen (device_name, "rb+");
    
    device_size = get_size(device);

    flag_success = write_superblock(pot, device_size, device);
    flag_success = flag_success && write_root_inode(device);

    fclose(device);
    return flag_success;
}

bool format(const char *device_name){
    char option, tp_form;
    unsigned int pot;

    std::cout << "Tem certeza que deseja formatar o despositivo? [S/N] ";
    std::cin >> option;
    if((option != 'S' && option != 's'))
        return false;

    std::cout << "Deseja fazer uma formatação completa? (Pode demorar, tipo muuuuuuuuuito mesmo) [S/N] ";
    std::cin >> tp_form;

    std::cout << "Potencia de 1024, para tamanho do bloco : ";
    std::cin >> pot;

    if(tp_form == 's' || tp_form == 'S')
        return hard_format(device_name, pot);
    
    return soft_format(device_name, pot);
}

#endif