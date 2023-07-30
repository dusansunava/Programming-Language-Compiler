#include <stdlib.h>
#include <stdint.h>
#include "tree.h"

#include "../../opperands.h"

tree *mkid(char *id)
{
    tree *t = mknode(t_id, 0, NULL, NULL);
    t -> value.idval = id;
    return t;
}

tree *mknum(int num)
{
    tree *t = mknode(t_num, 0, NULL, NULL);
    t -> value.numval = num;
    return t;
}

tree *mknode(treetype type, opperands op, tree *l, tree * r)
{
    tree *t = malloc(sizeof(tree));
    t -> type = type;
    t -> op = op;
    t -> left = l;
    t -> right = r;
    return t;
}