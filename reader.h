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


#endif