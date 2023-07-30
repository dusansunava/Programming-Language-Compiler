#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <cstring>
#include "../../opperands.h"
#include "mkcode.h"
#include "mkbinarycode.h"

void MkBinaryCode::mkBinaryCode(const std::vector<Opperand> &textForm, std::vector<uint32_t> &binaryForm) {
    for(const auto & i : textForm) {
        std::string workingOpperand = i.first;
        if((workingOpperand =="POP")) {
            uint32_t x = getVarFromMap(i.second);
            this->addOpToBinWInc(POP);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if((workingOpperand =="PUSH_CONST")) {
            uint32_t x = stoi(i.second);
            this->addOpToBinWInc(PUSH_CONST);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "PUSH_VAR") {
            uint32_t x = getVarFromMap(i.second);
            this->addOpToBinWInc(PUSH_VAR);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "ADD") {
            this->addOpToBinWInc(ADD);
        } else if(workingOpperand == "SUB") {
            this->addOpToBinWInc(SUB);
        } else if(workingOpperand == "MUL") {
            this->addOpToBinWInc(MUL);
        } else if(workingOpperand == "DIV") {
            this->addOpToBinWInc(DIV);
        } else if(workingOpperand == "GREATER") {
            this->addOpToBinWInc(GREATER);
        } else if(workingOpperand == "GREATER_EQUAL") {
            this->addOpToBinWInc(GREATER_EQUAL);
        } else if(workingOpperand == "LESS") {
            this->addOpToBinWInc(LESS);
        } else if(workingOpperand == "LESS_EQUAL") {
            this->addOpToBinWInc(LESS_EQUAL);
        } else if(workingOpperand == "EQUAL"){
            this->addOpToBinWInc(EQUAL);
        } else if(workingOpperand == "NOT_EQUAL"){
            this->addOpToBinWInc(NOT_EQUAL);
        } else if(workingOpperand == "A_INCREMENT") {        //increment and decrement was first meant to save result to heap in vm, later was it changed, only two of these OPs are used
            this->addOpToBinWInc(A_INCREMENT);
            uint32_t x = this->getVarFromMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "Z_INCREMENT") {
            this->addOpToBinWInc(Z_INCREMENT);
            uint32_t x = this->getVarFromMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "A_DECREMENT") {
            this->addOpToBinWInc(A_DECREMENT);
            uint32_t x = this->getVarFromMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "Z_DECREMENT") {
            this->addOpToBinWInc(Z_DECREMENT);
            uint32_t x = this->getVarFromMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "JUMPIFFALSE") {
            this->addOpToBinWInc(JUMPIFFALSE);
            uint16_t empty16 = 0;
            binaryFormInside.resize(binaryFormInside.size() + sizeof(empty16));
            memcpy(binaryFormInside.data() + this->offset, &empty16, sizeof(empty16));
            jumpMapFront[i.second] = this->offset;
            this->offset += sizeof(empty16);
        } else if(workingOpperand == "JUMPIFTRUE") {
            this->addOpToBinWInc(JUMPIFTRUE);
            uint16_t empty16 = 0;
            binaryFormInside.resize(binaryFormInside.size() + sizeof(empty16));
            memcpy(binaryFormInside.data() + this->offset, &empty16, sizeof(empty16));
            jumpMapFront[i.second] = this->offset;
            this->offset += sizeof(empty16);
        } else if(workingOpperand == "JUMP") {
            uint16_t empty16 = 0;
            this->addOpToBinWInc(JUMP);
            binaryFormInside.resize(binaryFormInside.size() + sizeof(empty16));
            memcpy(binaryFormInside.data() + this->offset, &empty16, sizeof(empty16));
            jumpMapFront[i.second] = this->offset;
            this->offset += sizeof(empty16);
        } else if(workingOpperand == "LOOP") {
            jumpMapFront[i.second] = this->offset;
        } else if(workingOpperand == "FUNCTION") {
            this->addOpToBinWInc(FUNCTION);
            functionMap[i.second] = offset;
        }else if(workingOpperand == "CALL") {
            this->addOpToBinWInc(CALL);
            uint16_t xx = functionMap.find(i.second)->second;
            binaryFormInside.resize(binaryFormInside.size() + sizeof(xx));
            memcpy(binaryFormInside.data() + this->offset, &xx, sizeof(xx));
            this->offset += sizeof(xx);
        }else if(workingOpperand == "END_FUN") {
            this->addOpToBinWInc(END_FUN);
        } else if(workingOpperand == "PRINT_STR") {
            const char *str = i.second.c_str();
            this->addOpToBinWInc(PRINT_STR);
            uint16_t len = (uint16_t)strlen(str);
            binaryFormInside.resize(binaryFormInside.size() + sizeof(len));
            memcpy(binaryFormInside.data() + this->offset, &len, sizeof(len));
            this->offset += sizeof(len);
            binaryFormInside.resize(binaryFormInside.size() + len);
            memcpy(binaryFormInside.data() + offset, str, len);
            this->offset += len;

        } else if(workingOpperand == "PRINT_VAR") {
            this->addOpToBinWInc(PRINT_VAR);
            uint32_t x = getVarFromMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "GET_INPUT"){
            this->addOpToBinWInc(GET_INPUT);
        } else if(workingOpperand == "DEFINE_GLOBAL") {
            this->addOpToBinWInc(DEFINE_GLOBAL);
            uint32_t x = addVarToGlobalMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "DEFINE_LOCAL") {
            this->addOpToBinWInc(DEFINE_LOCAL);
            uint32_t x = addVarToLocalMap(i.second);
            binaryFormInside.resize(sizeof(x) + binaryFormInside.size());
            memcpy(binaryFormInside.data() + offset, &x, sizeof(x));
            offset += sizeof(x);
        } else if(workingOpperand == "PUSH_RET") {
            this->addOpToBinWInc(PUSH_RET);
        } else if(workingOpperand == "POP_RET"){
            this->addOpToBinWInc(POP_RET);
        } else {
            bool found = false;
            for (auto label = jumpMapFront.begin(); label != jumpMapFront.end();) {
                    if(workingOpperand == label->first) {
                        int pos = label->second;
                        uint16_t xx = this->offset;
                        memcpy(binaryFormInside.data() + pos, &xx, sizeof(xx));
                        found = true;
                        label = jumpMapFront.erase(label);
                    } else {
                        ++label;
                    }
            }
            if(!found) {
                jumpMapBack[workingOpperand] = this->offset;
            }

        }
    }
    for (auto label = jumpMapBack.begin(); label != jumpMapBack.end();) {
        uint16_t pos = jumpMapFront.find(label->first)->second;
        uint16_t x = label->second;
        memcpy(binaryFormInside.data() + pos, &x, sizeof(x));
        label++;
    }

    addOpToBinWInc(END_PROGRAM);
}

//terminate if variable is already defined
void MkBinaryCode::checkUniqueName(const std::string &name) {
    auto search = varMapLocal.find(name);
    if(search != varMapLocal.end()) {
        std::cerr << "COMPILETIME ERROR: variable with name ," << name << "' is already defined!";
        exit(1);
    }
}


uint32_t MkBinaryCode::addVarToGlobalMap(const std::string &name) {
    this->checkUniqueName(name);
    if(varMapLocal.empty()) {
        varMapLocal[name] = this->globalVarValue;
        return this->globalVarValue--;
    }
    varMapLocal[name] = this->globalVarValue;
    return this->globalVarValue--;
}


uint32_t MkBinaryCode::addVarToLocalMap(const std::string &name) {
    this->checkUniqueName(name);
    if(varMapLocal.empty()) {
        varMapLocal[name] = this->varValue;
        return this->varValue++;
    }
    varMapLocal[name] = this->varValue;
    return this->varValue++;
}

//search first in local space, then in global space, returns -1 if variable is not defined
int MkBinaryCode::getVarFromMap(const std::string &name) {
    if(!varMapLocal.empty()) {
        auto search = varMapLocal.find(name);
        if(search != varMapLocal.end()) {
            return search->second;
        }
    }
    std::cerr << "COMPILETIME ERROR: variable with name ," << name << "' is not defined!";
    exit(1);
}

void MkBinaryCode::addOpToBinWInc(uint8_t ch) {
    binaryFormInside.resize(binaryFormInside.size() + sizeof(ch));
    memcpy(binaryFormInside.data() + offset, &ch, sizeof(ch));
    this->offset += sizeof(ch);
}
