#include <cstdio>
#include <vector>
#include <iostream>
#include <bitset>
#include "tree.h"
#include "mkcode.h"
#include "mkbinarycode.h"
#include "proceedTree.h"

extern "C" void proceed(tree *t, int currentMode, FILE * fp) {

    ProceedTree proceedTree;
    proceedTree.proceed(t, currentMode, fp);
};

void ProceedTree::proceed(tree *t, int currentMode, FILE *fp) {

    switch (currentMode) {
        case 1:
            mkCode.compile(t, textForm);
            mkBinaryCode.addBinaryFormVct(binaryFormNew);
            mkBinaryCode.mkBinaryCode(textForm, binaryForm);

            //write binary code to file
            for(int i = 0; i < mkBinaryCode.getBinaryCode().size(); i++) {
                fwrite(mkBinaryCode.getBinaryCode().data()+i, 1, 1, fp);
            }
            break;
        case 2:
            mkCode.compile(t, textForm);
            writeVectorToFile(textForm, fp);
            break;

    }
}

void ProceedTree::writeVectorToFile(std::vector<Opperand> &vector, FILE *fp) {
    for (const auto& s : vector) {
        fprintf(fp, (char*)s.first.c_str());
        if(!s.second.empty()) {
            fprintf(fp, " ");
            fprintf(fp, (char*)s.second .c_str());
        }
        fprintf(fp, "\n");
    }
}

//only for developing
void ProceedTree::printVector(std::vector<Opperand> &vector) {
    std::cout << "Print:\n";
    for (const auto& s : vector) {
        std::cout << s.first << " " << s.second << "\n";
    }
    std::cout << "\n";
}


