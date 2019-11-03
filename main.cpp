#include <stdio.h>

#include <bits/stdc++.h>
#include "structures.h"
#include "utils.h"
#include "format_controller.h"
#include "reader.h"

int main(int argc, const char **argv){
    const char *device_name;
    const char *directive = argv[1];

    if( !strcmp(directive,"--format") ){
        device_name = argv[2];
        if(format(device_name)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    return 0;
}