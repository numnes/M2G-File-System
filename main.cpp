#include <stdio.h>

#include <bits/stdc++.h>
#include "structures.h"
#include "utils.h"
#include "format_controller.h"
#include "reader.h"
#include "reader_controller.h"
#include "writer_controller.h"
#include "view_mode.h"
#include "prints.h"

int main(int argc, const char **argv){
    const char *device_name;
    const char *directive = argv[1];

    if(argc < 2){
        print_help();
        return 0;
    }
    else if( !strcmp(directive,"--format") || !strcmp(directive,"-f")){
        device_name = argv[2];
        if(format(device_name)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    else if( !strcmp(directive,"--import") || !strcmp(directive,"-i") ){
        const char *file_source_name = argv[2];
        const char *device_name = argv[3];
        const char *path_into_device = argv[4];
        bool root = argc < 5;
        if(import_file(file_source_name, device_name, path_into_device, root)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    else if(!strcmp(directive,"--view") || !strcmp(directive,"-v") ){
        const char *device_name = argv[2];
        view_mode(device_name);
        return 0;
    }
    else if(!strcmp(directive,"--help") || !strcmp(directive,"-h")){
        print_help();
        return 0;
    }
    else if(!strcmp(directive,"--makeDir") || !strcmp(directive,"-md")){
        const char *device_name = argv[2];
        const char *path_into_device = argv[3];

        if(mk_dir(device_name, path_into_device)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    else if(!strcmp(directive,"--remove") || !strcmp(directive,"-r")){
        const char *device_name = argv[2];
        const char *path_into_device = argv[3];

        if(rm_file(device_name, path_into_device)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    else if(!strcmp(directive,"--export") || !strcmp(directive,"-e")){
        const char *caminho_pasta_destino = argv[2];
        const char *device_name = argv[3];
        const char *path_into_device = argv[4];
        if(export_file(caminho_pasta_destino, device_name, path_into_device)){
            std::cout << "Sucesso!" << std::endl;
            return 0;
        }
        std::cout << "ERRO!" << std::endl;
        return 1;
    }
    else if(!strcmp(directive,"--link") || !strcmp(directive,"-l")) {
      const char *device_name = argv[2];
      const char *file_source_name = argv[3];
      const char *path_into_device = argv[4];
      if(link(device_name,file_source_name,path_into_device)){
        std::cout << "Sucesso!" << std::endl;
        return 0;
      }
      std::cout << "ERRO!" << std::endl;
      return 1;
    }
    else if(!strcmp(directive,"--hardLink") || !strcmp(directive,"-hl")) {
      const char *device_name = argv[2];
      const char *file_source_name = argv[3];
      const char *path_into_device = argv[4];
      if(hard_link(device_name,file_source_name,path_into_device)){
        std::cout << "Sucesso!" << std::endl;
        return 0;
      }
      std::cout << "ERRO!" << std::endl;
      return 1;
    }
    else{
        print_help();
        return 0;
    }
    return 0;
}
