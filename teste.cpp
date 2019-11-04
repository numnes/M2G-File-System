#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
#include <bits/stdc++.h>

void remove_final_dir(char *path){
    int size= strlen(path);
    int index_last_separator = 0;
    for(int i = 0; i < size; i++){
        if(path[i] == '/')
            index_last_separator = i;
    }
    if(index_last_separator == 0)
        return;
    char temp_str[index_last_separator];
    for(int i = 0; i < index_last_separator; i++){
        temp_str[i] = path[i];
    }
    memset(path, 0, size);
    strcpy(path, temp_str);
    path[index_last_separator] = '\0';
}

int main() {
    char teste[10];
    std::cin >> teste;
    std::cout << "str = " << teste << std::endl;
    remove_final_dir(teste);
    std::cout << "str = " << teste << std::endl;
    std::cout << "teste" << std::endl;
}