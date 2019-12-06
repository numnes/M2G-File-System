<<<<<<< HEAD
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
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes+ 1;
    unsigned int _indir = sb.num_inodes + 1;
    unsigned int free_position = empty_inode(device);
    unsigned int ad_new_inode = (sb.ad_inode_tab * block_size) + (free_position * 62);
    sb.num_free_inodes = sb.num_free_inodes - 1;

    fseek(device, 0, SEEK_SET);
    fwrite(&sb, sizeof(superblock), 1, device);

    inode new_inode;
    new_inode.type[0] = 1 ;
    new_inode.type[1] = 0 ;
    new_inode.size = 0;
    new_inode.last_access_time = local_time;
    new_inode.creation_time = local_time;
    new_inode.modified_time = local_time;
    new_inode.always_zero = 0;
    for(int i = 0; i < 8; i++)
        new_inode.direct_pointers[i] = null_ptr_dir;
    new_inode.inderect_pointer = null_ptr_indir;
    
    fseek(device, ad_new_inode, SEEK_SET);
    fwrite(&new_inode, sizeof(inode), 1, device);

    dec_free_inodes(device);

    return free_position;
}

bool inode_update(FILE *device, unsigned int inode_index, inode updated_inode){
    superblock sb;
    sb = read_superblock(device);

    unsigned int local_time = std::time(nullptr);
    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_updated_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);

    updated_inode.last_access_time = local_time;
    updated_inode.modified_time = local_time;
   
    fseek(device, ad_updated_inode, SEEK_SET);
    
    return fwrite(&updated_inode, sizeof(inode), 1, device);
}

bool init_root_dir(FILE *device, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    inode updated_inode;
    
    unsigned int new_block = get_empty_block(device);
    set_block_used(device, new_block);
    updated_inode.direct_pointers[0] = new_block;

    inode_update(device, 0, updated_inode);
}

unsigned long write_data_in_inode_from_file(FILE *device, FILE *data, unsigned int inode_index, unsigned long *data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    unsigned long file_length;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);


    while(*data_length > 0){
        
        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);
                        
            /////Gravação dos dados
            if(*data_length >= block_size){
                unsigned char buffer[block_size];
                fread(&buffer, block_size, 1, data);
                fwrite(&buffer, block_size, 1, device);
                *data_length -= block_size;
                updated_inode.size += block_size;
            }
            else{
                unsigned char buffer[*data_length]; 
                fread(&buffer, *data_length, 1, data);
                fwrite(&buffer, *data_length, 1, device);
                *data_length = 0;
                updated_inode.size += *data_length;
            }
            /////Gravação dos dados

            for(unsigned int j = 0; j < 8; j++){
                if(updated_inode.direct_pointers[j] == null_ptr_dir){
                    updated_inode.direct_pointers[j] = block;
                    direct = true;
                    break;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
                if(updated_inode.inderect_pointer == null_ptr_indir) // o ponteiro para o inode indireto não estava definido
                    updated_inode.inderect_pointer = inode_alloc(device);
                updated_inode.size = file_length;
                write_data_in_inode_from_file(device, data, updated_inode.inderect_pointer, data_length);
            }
        }else{
            updated_inode.size = file_length;
            inode_update(device, inode_index, updated_inode);
            return 0;
        }
    }
    updated_inode.size = file_length;
    inode_update(device, inode_index, updated_inode);
    return updated_inode.size;
}

bool write_in_file_from_inode(FILE *device, FILE *data, unsigned int inode_index, unsigned long *data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    for(unsigned int j = 0; j < 8; j++){
        if(updated_inode.direct_pointers[j] != null_ptr_dir){
            if(*data_length > block_size){
                fseek(device, (updated_inode.direct_pointers[j] * block_size), SEEK_SET);
                unsigned char buffer[block_size];
                fread(&buffer, block_size, 1, device);
                fwrite(&buffer, block_size, 1, data);
                *data_length -= block_size;
            }
            else{
                fseek(device, (updated_inode.direct_pointers[j] * block_size), SEEK_SET);
                unsigned char buffer[*data_length];
                fread(&buffer, *data_length, 1, device);
                fwrite(&buffer, *data_length, 1, data);
                *data_length = 0;
            }
        }
        else{
            return true;
        }
    }
    if(updated_inode.inderect_pointer != null_ptr_indir){
        write_in_file_from_inode(device, data, updated_inode.inderect_pointer, data_length);
    }
    return true;
}

bool write_directory_entry_in_inode(FILE *device, directory_entry de, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    inode updated_inode;
    
    char free_dir_name[27];
    memset(free_dir_name, 0, 27);

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);
    
    bool end_dir_pointers = false;

    for(int i = 0; i < 8; i++){
        if(updated_inode.direct_pointers[i] != null_ptr_dir){
            unsigned long pointer = (updated_inode.direct_pointers[i]) * block_size;
            fseek(device, pointer, SEEK_SET);
            for( int j = 0; j < block_size; j += sizeof(directory_entry) ){
                directory_entry temp_de;
                fread(&temp_de, sizeof(directory_entry), 1, device);
                if(temp_de._type == 3 || (temp_de._type == 0 && (!strcmp(temp_de.name, free_dir_name)))){
                    fseek(device, -1 * (sizeof(directory_entry)), SEEK_CUR);
                    fwrite(&de, sizeof(directory_entry), 1, device);
                    return true;
                }
            }
        }else{
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            set_block_used(device, block);

            updated_inode.direct_pointers[i] = block;
            inode_update(device, inode_index, updated_inode);
            fseek(device, ad_block, SEEK_SET);
            fwrite(&de, sizeof(directory_entry), 1, device);
            return true;
        }
    }

    if(updated_inode.inderect_pointer != null_ptr_indir){
        return write_directory_entry_in_inode(device, de, updated_inode.inderect_pointer);
    }
    
    unsigned int index_inode = inode_alloc(device);
    updated_inode.inderect_pointer = index_inode;
    inode_update(device, inode_index, updated_inode);
    return write_directory_entry_in_inode(device, de, updated_inode.inderect_pointer);
    
}

bool remove_dir_entry(FILE *device, char *path){
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

directory_entry create_dir_entry(FILE *device, unsigned char type, const char *name){
    directory_entry de;

    unsigned int index_inode = inode_alloc(device);

    de.index_inode = index_inode;
    de._type = type;
    strcpy(de.name, name);

    return de;
}

#endif
=======
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
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes+ 1;
    unsigned int _indir = sb.num_inodes + 1;
    unsigned int free_position = empty_inode(device);
    unsigned int ad_new_inode = (sb.ad_inode_tab * block_size) + (free_position * 62);
    sb.num_free_inodes = sb.num_free_inodes - 1;

    fseek(device, 0, SEEK_SET);
    fwrite(&sb, sizeof(superblock), 1, device);

    inode new_inode;
    new_inode.type[0] = 1 ;
    new_inode.type[1] = 0 ;
    new_inode.size = 0;
    new_inode.last_access_time = local_time;
    new_inode.creation_time = local_time;
    new_inode.modified_time = local_time;
    new_inode.link_count = 0;
    for(int i = 0; i < 8; i++)
        new_inode.direct_pointers[i] = null_ptr_dir;
    new_inode.inderect_pointer = null_ptr_indir;

    fseek(device, ad_new_inode, SEEK_SET);
    fwrite(&new_inode, sizeof(inode), 1, device);

    dec_free_inodes(device);

    return free_position;
}

bool inode_update(FILE *device, unsigned int inode_index, inode updated_inode){
    superblock sb;
    sb = read_superblock(device);

    unsigned int local_time = std::time(nullptr);
    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_updated_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);

    updated_inode.last_access_time = local_time;
    updated_inode.modified_time = local_time;

    fseek(device, ad_updated_inode, SEEK_SET);

    return fwrite(&updated_inode, sizeof(inode), 1, device);
}

bool init_root_dir(FILE *device, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    inode updated_inode;

    unsigned int new_block = get_empty_block(device);
    set_block_used(device, new_block);
    updated_inode.direct_pointers[0] = new_block;

    inode_update(device, 0, updated_inode);
}

unsigned long write_data_in_inode_from_file(FILE *device, FILE *data, unsigned int inode_index, unsigned long *data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);


    while(*data_length > 0){

        if(are_free_blocks(device)){
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            bool direct = false;

            set_block_used(device, block);
            fseek(device, ad_block, SEEK_SET);

            /////Gravação dos dados
            if(*data_length >= block_size){
                unsigned char buffer[block_size];
                fread(&buffer, block_size, 1, data);
                fwrite(&buffer, block_size, 1, device);
                *data_length -= block_size;
                updated_inode.size += block_size;
            }
            else{
                unsigned char buffer[*data_length];
                fread(&buffer, *data_length, 1, data);
                fwrite(&buffer, *data_length, 1, device);
                *data_length = 0;
                updated_inode.size += *data_length;
            }
            /////Gravação dos dados

            for(unsigned int j = 0; j < 8; j++){
                if(updated_inode.direct_pointers[j] == null_ptr_dir){
                    updated_inode.direct_pointers[j] = block;
                    direct = true;
                    break;
                }
            }
            if(!direct){ // todos os ponteiros diretos foram usados
                if(updated_inode.inderect_pointer == null_ptr_indir) // o ponteiro para o inode indireto não estava definido
                    updated_inode.inderect_pointer = inode_alloc(device);
                updated_inode.size += write_data_in_inode_from_file(device, data, updated_inode.inderect_pointer, data_length);
            }
        }else{
            inode_update(device, inode_index, updated_inode);
            return 0;
        }
    }
    inode_update(device, inode_index, updated_inode);
    return updated_inode.size;
}

bool write_in_file_from_inode(FILE *device, FILE *data, unsigned int inode_index, unsigned long *data_length){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    inode updated_inode;

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    for(unsigned int j = 0; j < 8; j++){
        if(updated_inode.direct_pointers[j] != null_ptr_dir){
            if(*data_length > block_size){
                fseek(device, (updated_inode.direct_pointers[j] * block_size), SEEK_SET);
                unsigned char buffer[block_size];
                fread(&buffer, block_size, 1, device);
                fwrite(&buffer, block_size, 1, data);
                *data_length -= block_size;
            }
            else{
                fseek(device, (updated_inode.direct_pointers[j] * block_size), SEEK_SET);
                unsigned char buffer[*data_length];
                fread(&buffer, *data_length, 1, device);
                fwrite(&buffer, *data_length, 1, data);
                *data_length = 0;
            }
        }
        else{
            return true;
        }
    }
    if(updated_inode.inderect_pointer != null_ptr_indir){
        write_in_file_from_inode(device, data, updated_inode.inderect_pointer, data_length);
    }
    return true;
}

bool write_directory_entry_in_inode(FILE *device, directory_entry de, unsigned int inode_index){
    superblock sb;
    sb = read_superblock(device);

    unsigned int block_size = (1024 << sb.pot_block_size);
    unsigned int ad_inode = (sb.ad_inode_tab * block_size) + (inode_index * 62);
    unsigned int null_ptr_dir = sb.num_blocks + 1;
    unsigned int null_ptr_indir = sb.num_inodes + 1;
    inode updated_inode;

    char free_dir_name[27];
    memset(free_dir_name, 0, 27);

    fseek(device, ad_inode, SEEK_SET);
    fread(&updated_inode, sizeof(inode), 1, device);

    bool end_dir_pointers = false;

    for(int i = 0; i < 8; i++){
        if(updated_inode.direct_pointers[i] != null_ptr_dir){
            unsigned long pointer = (updated_inode.direct_pointers[i]) * block_size;
            fseek(device, pointer, SEEK_SET);
            for( int j = 0; j < block_size; j += sizeof(directory_entry) ){
                directory_entry temp_de;
                fread(&temp_de, sizeof(directory_entry), 1, device);
                if(temp_de._type == 3 || (temp_de._type == 0 && (!strcmp(temp_de.name, free_dir_name)))){
                    fseek(device, -1 * (sizeof(directory_entry)), SEEK_CUR);
                    fwrite(&de, sizeof(directory_entry), 1, device);
                    return true;
                }
            }
        }else{
            unsigned int block = get_empty_block(device);
            unsigned long ad_block = block * block_size;
            set_block_used(device, block);

            updated_inode.direct_pointers[i] = block;
            inode_update(device, inode_index, updated_inode);
            fseek(device, ad_block, SEEK_SET);
            fwrite(&de, sizeof(directory_entry), 1, device);
            return true;
        }
    }

    if(updated_inode.inderect_pointer != null_ptr_indir){
        return write_directory_entry_in_inode(device, de, updated_inode.inderect_pointer);
    }

    unsigned int index_inode = inode_alloc(device);
    updated_inode.inderect_pointer = index_inode;
    inode_update(device, inode_index, updated_inode);
    return write_directory_entry_in_inode(device, de, updated_inode.inderect_pointer);

}

bool remove_dir_entry(FILE *device, char *path){
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

directory_entry create_dir_entry(FILE *device, unsigned char type, const char *name){
    directory_entry de;

    unsigned int index_inode = inode_alloc(device);

    de.index_inode = index_inode;
    de._type = type;
    strcpy(de.name, name);

    return de;
}

#endif
>>>>>>> 83e885888113b5220f80da434ea515673be6a2dc
