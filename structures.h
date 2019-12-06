#ifndef M2G_STRUCTURES
#define M2G_STRUCTURES

typedef struct superblock{
    unsigned int num_inodes;
    unsigned int num_blocks;
    unsigned int num_free_blocks;
    unsigned int num_free_inodes;
    unsigned int pot_block_size;
    unsigned int ad_block_bitmp;
    unsigned char num_blocks_block_bitmp;
    unsigned int ad_inode_tab;
    unsigned int ad_data_block;
} __attribute__((packed)) superblock;

typedef struct inode{
    unsigned char type[2];
    unsigned long size;
    unsigned int last_access_time;
    unsigned int creation_time;
    unsigned int modified_time;
    unsigned int always_zero;
    unsigned int direct_pointers[8];
    unsigned int inderect_pointer;
} __attribute__((packed)) inode;

typedef struct directory_entry{
    unsigned int index_inode;
    unsigned char _type;
    char name[27];
}  __attribute__((packed)) directory_entry;
#endif