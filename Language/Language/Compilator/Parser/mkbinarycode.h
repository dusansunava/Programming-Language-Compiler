#ifndef COMPILER_MKBINARYCODE_H
#define COMPILER_MKBINARYCODE_H

#include <vector>
#include <map>

struct MkBinaryCode {
private:
    uint32_t addVarToGlobalMap(const std::string& name);
    uint32_t addVarToLocalMap(const std::string& name);
    int getVarFromMap(const std::string& name);
    void checkUniqueName(const std::string& name);
    uint32_t varValue = 0;
    uint32_t globalVarValue = -1; //set to max uint32_t value
    int offset;
    std::vector<uint8_t> binaryFormInside;
    void addOpToBinWInc(uint8_t);

public:
    MkBinaryCode(){
        this->offset = 0;
    }

    std::map<std::string , uint32_t> varMapLocal;
    std::map<std::string, int> jumpMapFront;
    std::map<std::string, int> jumpMapBack;
    std::map<std::string, int> functionMap;
    void addBinaryFormVct(std::vector<uint8_t> vct) {
        this->binaryFormInside = vct;
    }
    void mkBinaryCode(const std::vector<Opperand>& textForm, std::vector<uint32_t>& binaryForm);
    std::vector<uint8_t> getBinaryCode (){
        return this->binaryFormInside;
    };
};

#endif
