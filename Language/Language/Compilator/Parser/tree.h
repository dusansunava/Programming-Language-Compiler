#include <stdlib.h>
#pragma once

typedef enum
{
    t_block,
    t_join,
    t_assign,
    t_num,
    t_id,
    t_op,
    t_if,
    t_then,
    t_else,
    t_loop,
    t_loop_while,
    t_do,
    t_while,
    t_function,
    t_function_call,
    t_function_def,
    t_print_str,
    t_print_var,
    t_get_input,
    t_parameter_pop,
    t_parameter_push,
    t_global_var,
    t_local_var,
    t_function_ret,
    t_function_call_w_ret

} treetype;

typedef enum
{
    a_increment,
    z_increment,
    a_decrement,
    z_decrement,
    plus,
    minus,
    greater,
    greater_equal,
    less,
    less_equal,
    multiply,
    divison,
    none,
    equal,
    not_equal
} opperands;

typedef struct t
{
    treetype type;
    opperands op;
    union
    {
        int numval;
        char *idval;
    } value;

    struct t *left;
    struct t *right;
} tree;

tree *mknum(int);
tree *mkid(char *);
tree *mknode(treetype, opperands, tree *, tree *);