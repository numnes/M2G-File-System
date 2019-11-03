#ifndef M2G_WRITER
#define M2G_WRITER

#include <bits/stdc++.h>
#include "structures.h"
#include "reader.h"

unsigned int inode_alloc(FILE *device){
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

bool inode_update(FILE *device, unsigned int inode_index, inode updated_inode){
    superblock sb;
    sb = read_superblock(device);

    unsigned int local_time = std::time(nullptr);
    unsigned int ad_updated_inode = sb.ad_inode_tab + (inode_index * 64);

    updated_inode.last_access_time = local_time;
    updated_inode.modified_time = local_time;
   
    fseek(device, ad_updated_inode, SEEK_SET);
    
    return fwrite(&updated_inode, sizeof(inode), 1, device);
}

bool write_data_in_inode(FILE *device, unsigned int inode_index, unsigned char *data, unsigned long data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = sb.ad_inode_tab + (inode_index * 64);
    unsigned int null_ptr = (num_blocks(device) + 1) * block_size;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    for(unsigned long i = 0; i < data_length;){
        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);
            
            i += block_size;
            /////Gravação dos dados
            for(unsigned long j = 0; j < block_size && j < data_length; j++){

                //melhorar essa parte, aqui é esperado todos os dados em uma string de char, e os dados são gravados byte a byte
                //tem que otimizar, fiz assim só pra possibilitar a formatação, o resto da função ta de boas.

                //já fiz isso ^, criando as duas funções aqui abaixo, vou manter essa função aqui só por exemplo por estar comentada pq 
                //a parte de alocação de blocos apontados no bloco do ponteiro indireto nunca é usada aqui, ela só é chamada na 
                //formatação quando o diretorio raiz é criado.
                fwrite(&data[j], sizeof(unsigned char), 1, device);
            }
            /////Gravação dos dados


            for(unsigned int j = 0; j < 8; j++){
                if(updated_inode.direct_pointers[j] == null_ptr){
                    updated_inode.direct_pointers[j] = block * block_size;
                    direct = true;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
            std::cout << "ta aqui mas não era pra tá" << std::endl;
                if(updated_inode.inderect_pointer == null_ptr){
                    unsigned int new_block_indirect = get_empty_block(device);
                    unsigned long ad_block_indirect = block * block_size;

                    set_block_used(device, new_block_indirect);
                    fseek(device, ad_block_indirect, SEEK_SET);
                    fwrite(&new_block_indirect, sizeof(unsigned int), 1, device);
                }
                else{
                    unsigned int num_indirect_pointers = ceil(updated_inode.size / block_size);
                    unsigned long adress_new_indirect_pointer = (updated_inode.inderect_pointer * block_size) + (num_indirect_pointers * 4);
                    
                    fseek(device, adress_new_indirect_pointer, SEEK_SET);
                    fwrite(&block, sizeof(unsigned int), 1, device);
                }
            }
            updated_inode.size += data_length;
        }else{
            return false;
        }
    }
    return inode_update(device, inode_index, updated_inode);
}

bool write_data_in_inode_from_file(FILE *device, FILE *data, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = sb.ad_inode_tab + (inode_index * 64);
    unsigned int null_ptr = (num_blocks(device) + 1) * block_size;
    unsigned long data_length = get_size(data);
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    while(data_length > 0){
        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);
                        
            /////Gravação dos dados
            if(data_length >= block_size){
                unsigned char buffer[block_size];              
                fread(&buffer, block_size, 1, data);
                fwrite(&buffer, block_size, 1, device);
                data_length -= block_size;
                updated_inode.size += block_size;
            }
            else{
                unsigned char buffer[data_length];              
                fread(&buffer, data_length, 1, data);
                fwrite(&buffer, data_length, 1, device);
                data_length = 0;
                updated_inode.size += data_length;
            }
            /////Gravação dos dados

            for(unsigned int j = 0; j < 8; j++){
                if(updated_inode.direct_pointers[j] == null_ptr){
                    updated_inode.direct_pointers[j] = block * block_size;
                    direct = true;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
                if(updated_inode.inderect_pointer == null_ptr){
                    unsigned int new_block_indirect = get_empty_block(device);
                    unsigned long ad_block_indirect = block * block_size;

                    set_block_used(device, new_block_indirect);
                    fseek(device, ad_block_indirect, SEEK_SET);
                    fwrite(&new_block_indirect, sizeof(unsigned int), 1, device);
                }
                else{
                    unsigned int num_indirect_pointers = ceil(updated_inode.size / block_size);
                    unsigned long adress_new_indirect_pointer = (updated_inode.inderect_pointer * block_size) + (num_indirect_pointers * 4);
                    
                    fseek(device, adress_new_indirect_pointer, SEEK_SET);
                    fwrite(&block, sizeof(unsigned int), 1, device);
                }
            }
        }else{
            return false;
        }
    }
    
    return inode_update(device, inode_index, updated_inode);;
}

bool write_directory_entry_in_inode(FILE *device, directory_entry de, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = sb.ad_inode_tab + (inode_index * 64);
    unsigned int null_ptr = (num_blocks(device) + 1) * block_size;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    unsigned int index_pointer = (updated_inode.size / block_size);
    unsigned int index_byte = updated_inode.size % block_size;
    bool end_dir_pointers = false;

    if(index_pointer < 8){
        if((index_byte + sizeof(directory_entry)) < (updated_inode.direct_pointers[index_pointer] + block_size)){
            
            fseek(device, ad_inode, SEEK_SET);
        }
    }
    





    while(data_length > 0){
        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);
                        
            /////Gravação dos dados
            if(data_length >= block_size){
                unsigned char buffer[block_size];              
                fread(&buffer, block_size, 1, data);
                fwrite(&buffer, block_size, 1, device);
                data_length -= block_size;
                updated_inode.size += block_size;
            }
            else{
                unsigned char buffer[data_length];              
                fread(&buffer, data_length, 1, data);
                fwrite(&buffer, data_length, 1, device);
                data_length = 0;
                updated_inode.size += data_length;
            }
            /////Gravação dos dados

            for(unsigned int j = 0; j < 8; j++){
                if(updated_inode.direct_pointers[j] == null_ptr){
                    updated_inode.direct_pointers[j] = block * block_size;
                    direct = true;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
                if(updated_inode.inderect_pointer == null_ptr){
                    unsigned int new_block_indirect = get_empty_block(device);
                    unsigned long ad_block_indirect = block * block_size;

                    set_block_used(device, new_block_indirect);
                    fseek(device, ad_block_indirect, SEEK_SET);
                    fwrite(&new_block_indirect, sizeof(unsigned int), 1, device);
                }
                else{
                    unsigned int num_indirect_pointers = ceil(updated_inode.size / block_size);
                    unsigned long adress_new_indirect_pointer = (updated_inode.inderect_pointer * block_size) + (num_indirect_pointers * 4);
                    
                    fseek(device, adress_new_indirect_pointer, SEEK_SET);
                    fwrite(&block, sizeof(unsigned int), 1, device);
                }
            }
        }else{
            return false;
        }
    }
    return inode_update(device, inode_index, updated_inode);;
}

directory_entry create_dir_entry(FILE *device, unsigned char type, const char *name){
    directory_entry de;

    unsigned int index_inode = inode_alloc(device);

    de.index_inode = index_inode;
    de._type = type;
    strcpy(de.name, name);

    return de;
}

#endif