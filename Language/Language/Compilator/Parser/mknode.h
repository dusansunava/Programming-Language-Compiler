typedef enum
{
    t_block,
    t_join,
    t_if,
    t_then,
    t_else,
    t_assign,
    t_op,
    t_num,
    t_id,
    t_loop,
    t_do,
    t_while

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
    none
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

