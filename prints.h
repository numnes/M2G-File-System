#ifndef M2G_PRINTS
#define M2G_PRINTS

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

void print_sobre(){
    std::cout << "\t __  __ ____   ____\n\t|  \\/  |___ \\ / ___|\n\t| |\\/| | __) | |  _\n\t| |  | |/ __/| |_| |\n\t|_|  |_|_____|\\____|\n";
}

void print_help(){
    print_sobre();
    std::cout << "============FILE-SYSTEM============" << std::endl;
    std::cout << "\n";
    std::cout << "Os comandos abaixo indicam as ações válidas:" << std::endl<< std::endl;

    std::cout << "\t# ./m2g [-f ou --format] [caminhoDisco] -> Formata o disco no padrão M2G." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-i ou --import] [caminhoParaArquivoSerCopiado] [caminhoDisco] [CaminhoPastaDestinoDentroDoDisco] -> Copia um arquivo para o disco." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-e ou --export] [caminhoPastaDestino] [caminhoDisco] [caminhoArquivoDisco] -> Exporta um arquivo." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-md ou --makeDir] [caminhoDisco] [caminhoNovoDiretorio]/[nomeDiretorio] -> Cria um novo diretorio." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-l ou --link] [caminhoDisco] [caminhoArquivoLinkado] [caminhoDoNovoLink] -> Cria um softlink." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-hl ou --hardLink] [caminhoDisco] [caminhoArquivoLinkado] [caminhoDoNovoLink] -> Cria um hard-link." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-v ou --view] [caminhoDisco] -> Abre um disco para navegação." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-r ou --remove] [caminhoDisco] [caminhoDoArquivoASerRemovidoNoDisco] -> Deleta um arquivo ou diretorio no disco." << std::endl<< std::endl;
    std::cout << "\t# ./m2g [-h ou --help] -> Exibe o menu de ajuda." << std::endl;
}
#endif