#ifndef MKNODE_HEADER
#define MKNODE_HEADER

#include <utility>
#include <vector>
#include "tree.h"

struct Opperand{

    Opperand(std::string string, std::string string1) {
        this->first = std::move(string);
        this->second = std::move(string1);
    };
    explicit Opperand(std::string string) {
        this->first = std::move(string);
    };

    std::string first;
    std::string second;
};

struct MkCode {

public:
    
    void compile(tree * t, std::vector<Opperand> &data);
private:
    int labelCounter = 0;
    std::vector<int> labelCounterBack;
    std::vector<int> labelCounterBackSec;
};

#endif