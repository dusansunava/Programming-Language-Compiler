#include <vector>
#include <string>
#include <iostream>
#include "mkcode.h"

void MkCode::compile(tree *t, std::vector<Opperand> &data) {

    if(!t) return;
    switch (t->type) {
        case t_join:
            compile(t->left, data);
            if(t->right) compile(t->right, data);
            break;
        case t_assign:
            compile(t->right, data);
            data.emplace_back("POP", t->left->value.idval);
            break;
        case t_num:
            data.emplace_back("PUSH_CONST", std::to_string(t->value.numval));
            break;
        case t_id:
            data.emplace_back("PUSH_VAR", t->value.idval);
            break;
        case t_op:
            compile(t->left, data);
            compile(t->right, data);
            switch (t->op) {
                case plus:
                    data.emplace_back("ADD");
                    break;
                case minus:
                    data.emplace_back("SUB");
                    break;
                case multiply:
                    data.emplace_back("MUL");
                    break;
                case divison:
                    data.emplace_back("DIV");
                    break;
                case greater:
                    data.emplace_back("GREATER");
                    break;
                case greater_equal:
                    data.emplace_back("GREATER_EQUAL");
                    break;
                case less:
                    data.emplace_back("LESS");
                    break;
                case less_equal:
                    data.emplace_back("LESS_EQUAL");
                    break;
                case equal:
                    data.emplace_back("EQUAL");
                    break;
                case not_equal:
                    data.emplace_back("NOT_EQUAL");
                    break;
                case a_increment:
                    data.pop_back();
                    data.emplace_back("A_INCREMENT", t->left->value.idval);
                    break;
                case z_increment:
                    data.pop_back();
                    data.emplace_back("Z_INCREMENT", t->right->value.idval);
                    break;
                case a_decrement:
                    data.pop_back();
                    data.emplace_back("A_DECREMENT", t->left->value.idval);
                    break;
                case z_decrement:
                    data.pop_back();
                    data.emplace_back("Z_DECREMENT", t->right->value.idval);
                    break;
                default:
                    data.emplace_back("UNHANDLED OPPERAND");
            }
            break;
        case t_if:
            compile(t->left, data);
            data.emplace_back("JUMPIFFALSE", "L" + std::to_string(labelCounter));
            labelCounterBack.push_back(labelCounter);
            labelCounter++;
            compile(t->right, data);
            break;

        case t_then:
            compile(t->left, data);

            data.emplace_back("JUMP", "L" + std::to_string(labelCounter));
            labelCounterBackSec.push_back(labelCounter);
            labelCounter++;

            data.emplace_back("L" + std::to_string(labelCounterBack.back()));
            labelCounterBack.pop_back();
            if(t->right) compile(t->right, data);
            data.emplace_back("L" + std::to_string(labelCounterBackSec.back()));
            labelCounterBackSec.pop_back();
            break;
        case t_loop:
            data.emplace_back( "L" + std::to_string(labelCounter));
            compile(t->left, data);
            compile(t->right, data);
            data.emplace_back("JUMPIFTRUE", "L" + std::to_string(labelCounter));
            labelCounter+=2;
            break;
        case t_loop_while:
            labelCounterBack.push_back(labelCounter);
            data.emplace_back( "L" + std::to_string(labelCounterBack.back()));
            compile(t->left, data);
            labelCounter++;
            data.emplace_back("JUMPIFFALSE", "L" + std::to_string(labelCounter));
            compile(t->right, data);
            data.emplace_back("JUMP", "L" + std::to_string(labelCounterBack.back()));
            labelCounterBack.pop_back();
            data.emplace_back( "L" + std::to_string(labelCounter));
            labelCounter++;
            break;
        case t_function:
            compile(t->left, data);
            compile(t->right, data);
            data.emplace_back("END_FUN");
            break;
        case t_function_def:
            data.emplace_back("FUNCTION", t->left->value.idval);
            if(t->right) compile(t->right, data);
            break;
        case t_function_call:
            if(t->right) compile(t->right, data);
            data.emplace_back("CALL", t->left->value.idval);
            break;
        case t_print_str:
            data.emplace_back("PRINT_STR", t->left->value.idval);
            break;
        case t_print_var:
            data.emplace_back("PRINT_VAR", t->left->value.idval);
            break;
        case t_get_input:
            data.emplace_back("GET_INPUT");
            break;
        case t_parameter_pop:
            data.emplace_back("DEFINE_LOCAL", t->left->value.idval);
            data.emplace_back("POP", t->left->value.idval);
            if(t->right) compile(t->right, data);
            break;
        case t_parameter_push:
            if(t->right) compile(t->right, data);
            data.emplace_back("PUSH_VAR", t->left->value.idval);
            break;
        case t_global_var:
            data.emplace_back("DEFINE_GLOBAL", t->left->value.idval);
            break;
        case t_local_var:
            data.emplace_back("DEFINE_LOCAL", t->left->value.idval);
            break;
        case t_function_ret:
            compile(t->left, data);
            data.emplace_back("POP_RET");
            break;
        case t_function_call_w_ret:
            compile(t->right, data);
            data.emplace_back("PUSH_RET");
            data.emplace_back("POP", t->left->value.idval);
            break;
    }
}
