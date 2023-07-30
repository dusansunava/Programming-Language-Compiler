#ifndef COMPILER_PROCEEDTREE_H
#define COMPILER_PROCEEDTREE_H

#include <vector>
#include <cstdint>
#include <cstdio>
#include "mkcode.h"

class ProceedTree {

public:

    void proceed(tree *t, int currentMode, FILE * fp);
    void printVector(std::vector<Opperand> &vector);

    std::vector<Opperand> textForm;
    std::vector<uint32_t> binaryForm;
    std::vector<uint8_t> binaryFormNew;
private:
    void writeVectorToFile(std::vector<Opperand> &vector, FILE * fp);
    MkCode mkCode;
    MkBinaryCode mkBinaryCode  ;
};


#endif
