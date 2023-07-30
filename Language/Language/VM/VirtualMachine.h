#ifndef PARSER_VIRTUALMACHINE_H
#define PARSER_VIRTUALMACHINE_H

#include <utility>
#include <vector>
#include <iostream>
#include <map>

class VirtualMachine {
private:
    std::vector<uint8_t> inputBinary;
    int offset = 0;

    std::vector<uint32_t> heap;
    std::vector<uint32_t> jumpHeap;
    uint32_t functionRetReg = 0;
    std::map<uint32_t , uint32_t> varMapGlobal;
    std::vector<std::map<uint32_t , uint32_t>> varMapLocal;

    uint32_t addVarToGlobalMap(uint32_t);
    uint32_t addVarToLocalMap(uint32_t);
    void popValueToVar(uint32_t , uint32_t);
    uint32_t getValueOfVar(uint32_t);

public:
    void cycle();
    void addInputBinary(std::vector<uint8_t> inputBin) {
        this->inputBinary = std::move(inputBin);
    };

    //only for developing
    void printHeap() {
        printf("Heap: \n");
        for (auto i = this->heap.begin(); i != this->heap.end(); ++i) {
            std::cout << *i << " | ";
        }
        printf("\n");
    }

    //only for developing
    void printVarMap() {
        printf("VarMapLocal: \n");
        for(auto const& y : this->varMapLocal) {
            for(auto const& x: y) {
                printf("Key: %d, value: %d\n", x.first, x.second);
            }
            printf("------------\n");
        }
        printf("/////////////////////// \n");

        /*
        for(auto const& x : this->varMap) {
            printf("Key: %d, value: %d\n", x.first, x.second);
        }
         */
    }

    //only for developing
    void printVarMapFun() {
        printf("VarMapGlobal: \n");
        for(auto const& x : this->varMapGlobal) {
            printf("Key: %d, value: %d\n", x.first, x.second);
        }
    }

};

#endif
