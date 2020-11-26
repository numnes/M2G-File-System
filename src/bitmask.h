#ifndef M2G_BITMASK
#define M2G_BITMASK

bool return_bit_at(unsigned char *bitmap, int bit_pos){
    int byte_pos = bit_pos / 8;
    int in_byte_pos = bit_pos % 8;
    unsigned char masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};   
    return (bitmap[byte_pos] & masks[in_byte_pos]);
}

bool toggle_bit_at(unsigned char *bitmap, int bit_pos, bool state){
    int byte_pos = bit_pos / 8;
    int in_byte_pos = bit_pos % 8;
    unsigned char masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    if(state)
        return bitmap[byte_pos] = bitmap[byte_pos] | masks[in_byte_pos];
    return bitmap[byte_pos] = bitmap[byte_pos] & (~(masks[in_byte_pos]));
}

#endif