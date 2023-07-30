#include <cstdint>
#include <string>
#include <cstring>
#include "VirtualMachine.h"
#include "../opperands.h"

uint32_t VirtualMachine::addVarToGlobalMap(uint32_t adress) {

    this->varMapGlobal[adress] = 0;
    return 0;
}

uint32_t VirtualMachine::addVarToLocalMap(uint32_t adress) {

    this->varMapLocal.back()[adress] = 0;
    return 0;
}

void VirtualMachine::popValueToVar(uint32_t value, uint32_t adress) {

    auto search = varMapLocal.back().find(adress);
    if(search != varMapLocal.back().end()) {
        this->varMapLocal.back()[adress] = value;
        return;
    }
    search = varMapGlobal.find(adress);
    if(search != varMapGlobal.end()) {
        this->varMapGlobal[adress] = value;
        return;
    }

    std::cout << "\033[1;31m";
    std::cerr << "RUNTIME ERROR: Undefined variable!\n";
    std::cout << "\033[0m";
    exit(0);
}

uint32_t VirtualMachine::getValueOfVar(uint32_t adress) {
    auto search = varMapLocal.back().find(adress);
    if(search != varMapLocal.back().end()) {
        return search->second;
    }
    search = varMapGlobal.find(adress);
    if(search != varMapGlobal.end()) {
        return search->second;
    }

    std::cout << "\033[1;31m";
    uint8_t wo;
    std::cerr << "RUNTIME ERROR: Undefined variable!\n";
    std::cout << "\033[0m";
    exit(0);
}

void VirtualMachine::cycle() {

    uint32_t a;
    uint32_t b;

    std::map<uint32_t ,uint32_t> m;
    this->varMapLocal.push_back(m);

    bool running = true;
    uint16_t x_16;
    uint32_t x_32;
    offset = 0;
    while (running) {

        uint8_t wo;
        memcpy(&wo, (this->inputBinary.data() + offset), sizeof(uint8_t));

        //jump at end of function definition
        if(wo == FUNCTION) {
            while(wo != END_FUN) {
                offset++;
                memcpy(&wo, (this->inputBinary.data() + offset), sizeof(uint8_t));
            }
            offset++;
            memcpy(&wo, (this->inputBinary.data() + offset), sizeof(uint8_t));
        }

        switch (wo) {
            case END_PROGRAM:
                running = false;
                break;
            case DEFINE_GLOBAL:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);
                this->addVarToGlobalMap(x_32);
                break;
            case DEFINE_LOCAL:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);
                this->addVarToLocalMap(x_32);
                break;
            case POP:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);

                this->popValueToVar(this->heap.back(), x_32);
                this->heap.pop_back();
                break;
            case PUSH_CONST:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);
                this->heap.push_back(x_32);
                break;
            case PUSH_VAR:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);
                a = this->getValueOfVar(x_32);
                this->heap.push_back(a);
                break;
            case ADD:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b + a);
                offset++;
                break;
            case SUB:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b - a);
                offset++;
                break;
            case MUL:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b * a);
                offset++;
                break;
            case DIV:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b / a);
                offset++;
                break;
            case GREATER:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b > a);
                offset++;
                break;
            case GREATER_EQUAL:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b >= a);
                offset++;
                break;
            case LESS:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b < a);
                offset++;
                break;
            case LESS_EQUAL:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(b <= a);
                offset++;
                break;
            case EQUAL:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(a == b);
                offset++;
                break;
            case NOT_EQUAL:
                a = this->heap.back();
                this->heap.pop_back();
                b = this->heap.back();
                this->heap.pop_back();
                this->heap.push_back(a != b);
                offset++;
                break;
            case A_INCREMENT:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                a = this->getValueOfVar(x_32);
                a++;
                this->popValueToVar(a, x_32);
                offset += sizeof(x_32);
                break;
            case A_DECREMENT:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                a = this->getValueOfVar(x_32);
                a--;
                this->popValueToVar(a, x_32);
                offset += sizeof(x_32);
                break;
            case JUMPIFFALSE:
                a = this->heap.back();
                this->heap.pop_back();
                offset++;
                if(!a) {
                    memcpy(&x_16, inputBinary.data() + offset, sizeof(x_16));
                    offset = x_16;
                } else {
                    offset += sizeof(x_16);
                }
                break;
            case JUMPIFTRUE:
                a = heap.back();
                this->heap.pop_back();
                offset++;
                if(a) {
                    memcpy(&x_16, inputBinary.data() + offset, sizeof(x_16));
                    offset = x_16;
                }  else {
                    offset += sizeof(x_16);
                }
                break;
            case JUMP:
                offset++;
                memcpy(&x_16, inputBinary.data() + offset, sizeof(x_16));
                offset = x_16;
                break;
            case CALL:
                this->varMapLocal.push_back({});
                offset++;
                memcpy(&x_16, inputBinary.data() + offset, sizeof(x_16));
                offset += sizeof(x_16);

                this->jumpHeap.push_back(offset);
                offset = x_16;
                break;
            case END_FUN:
                this->varMapLocal.pop_back();
                offset = this->jumpHeap.back();
                this->jumpHeap.pop_back();
                break;
            case PRINT_STR:
                char *str;
                offset++;
                uint16_t len;
                memcpy(&len, inputBinary.data() + offset, sizeof(len));
                offset += sizeof(len);
                str = (char*) malloc(len);
                memcpy(str, inputBinary.data() + offset, len);
                str[len] = '\0';
                char* substr;
                substr = (char*) malloc(len-2);
                std::strncpy(substr, &str[1], len-2);
                substr[len-2] = '\0';
                std::cout << substr;
                std::cout << "\n";
                offset+=len;
                break;
            case PRINT_VAR:
                offset++;
                memcpy(&x_32, inputBinary.data() + offset, sizeof(x_32));
                offset += sizeof(x_32);
                a = this->getValueOfVar(x_32);
                std::cout << a << "\n";
                break;
            case POP_RET:
                this->functionRetReg = this->heap.back();
                this->heap.pop_back();
                this->varMapLocal.pop_back();
                offset = this->jumpHeap.back();
                this->jumpHeap.pop_back();
                //offset++;
                break;
            case PUSH_RET:
                this->heap.push_back(this->functionRetReg);
                offset++;
                break;
            case GET_INPUT:
                std::string inputBuf;
                std::getline( std::cin, inputBuf );
                char* inputChar = new char[inputBuf.length() + 1];
                strcpy(inputChar, inputBuf.c_str());
                while(!std::isdigit(*inputChar)) {
                    std::cout << "WARNING: Input must be digit.\n";
                    std::getline( std::cin, inputBuf );
                    inputChar = new char[inputBuf.length() + 1];
                    strcpy(inputChar, inputBuf.c_str());
                }
                this->heap.push_back(std::atoi(inputChar));
                offset += 1;
                break;
        }
    }
}
