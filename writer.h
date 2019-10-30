#ifndef M2G_WRITER
#define M2G_WRITER

#include <bits/stdc++.h>
#include "structures.h"
#include "reader.h"

unsigned int create_inode(FILE *device){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int local_time = std::time(nullptr);
    unsigned int null_ptr = (num_blocks(device) + 1) * block_size;
    unsigned int free_position = empty_inode(device);
    unsigned int ad_new_inode = sb.ad_inode_tab + (free_position * 64);

    inode new_inode;
    new_inode.type[0] = 1 ;
    new_inode.type[1] = 0 ;
    new_inode.size = 0;
    new_inode.last_access_time = local_time;
    new_inode.creation_time = local_time;
    new_inode.modified_time = local_time;
    new_inode.always_zero = 0;
    for(int i = 0; i < 8; i++)
        new_inode.direct_pointers[i] = null_ptr;
    new_inode.inderect_pointer = null_ptr;
    
    fseek(device, ad_new_inode, SEEK_SET);
    fwrite(&new_inode, sizeof(inode), 1, device);

    return free_position;
}

bool write_data_in_inode(FILE *device, unsigned int inode_index, unsigned char *data, unsigned long data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = sb.ad_inode_tab + (inode_index * 64);
    unsigned int null_ptr = (num_blocks(device) + 1) * block_size;
    inode inode_at;

    fseek(device, ad_inode, SEEK_SET);
    fread(&inode_at, sizeof(inode), 1, device);

    for(unsigned long i = 0; i < data_length;){
        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            //set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);
            
            i += block_size;
            for(unsigned long j = 0; j < block_size && j < data_length; j++){

                //melhorar essa parte, aqui é esperado todos os dados em uma string de char, e os dados são gravados byte a byte
                //tem que otimizar, fiz assim só pra possibilitar a formatação, o resto da função ta de boas.
                fwrite(&data[j], sizeof(unsigned char), 1, device);
            }
            for(unsigned int j = 0; j < 8; j++){
                if(inode_at.direct_pointers[j] == null_ptr){
                    inode_at.direct_pointers[j] = block * block_size;
                    direct = true;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
            std::cout << "ta aqui mas não era pra tá" << std::endl;
                if(inode_at.inderect_pointer == null_ptr){
                    unsigned int new_block_indirect = get_empty_block(device);
                    unsigned long ad_block_indirect = block * block_size;

                    set_block_used(device, new_block_indirect);
                    fseek(device, ad_block_indirect, SEEK_SET);
                    fwrite(&new_block_indirect, sizeof(unsigned int), 1, device);
                }
                else{
                    unsigned int num_indirect_pointers = ceil(inode_at.size / block_size);
                    unsigned long adress_new_indirect_pointer = (inode_at.inderect_pointer * block_size) + (num_indirect_pointers * 4);
                    
                    fseek(device, adress_new_indirect_pointer, SEEK_SET);
                    fwrite(&block, sizeof(unsigned int), 1, device);
                }
            }
            inode_at.size += data_length;
        }else{
            return false;
        }
    }
    return true;
}



#endif