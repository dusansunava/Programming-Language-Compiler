#include "VirtualMachine.h"

#include <fstream>
#include <iostream>
#include <string>
#include <getopt.h>
#include <cstring>


int main(int argc, char *argv[]){

    int opt;
    int i;
    char inputFileName[100];

    while((opt = getopt(argc, argv, ":f:tbv:o:")) != -1) {
        switch(opt) {
            case 'o':
                for(i = 0; i < strlen(optarg); i++){
                    inputFileName[i] = optarg[i];
                }
                inputFileName[i++] = '\0';
                break;
        }
    }

    //check if input file is defined
    if(strlen(inputFileName) == 0) {
        std::cout << "Give input file name as argument using -o.";
        exit(1);
    }

    //check if input file exist
    std::ifstream input(inputFileName, std::ios::binary);
    if(!input) {
        std::cout << "Falied to open file with given name.";
        exit(1);
    }

    std::vector<uint8_t> buffer;
    input.seekg(0, std::ios::end);
    size_t filesize=input.tellg();
    input.seekg(0, std::ios::beg);

    //return if input file is empty
    if(filesize == 0) {
        return 0;
    }

    //copy input file to vector
    buffer.resize(filesize/sizeof(uint8_t));
    input.read((char *) buffer.data(), filesize);

    VirtualMachine vm;
    vm.addInputBinary(buffer);
    vm.cycle();

    return 0;
}