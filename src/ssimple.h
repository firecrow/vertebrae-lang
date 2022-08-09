#define STDIN 0 
#define STDOUT 1
#define STDERR 2 

#define bool char

enum TYPE {
    NONE = 0,
    CELL, 
    STRING,
    SYMBOL
};

struct string;
struct tree_node;
struct tree;
struct tree_iter;
struct symbol;
struct closure;
struct function;
struct cell;

struct tree_node {
    int is_black;
    void *content;
};

struct tree {
    enum TYPE type;
    struct root *tree_node; 
    int count;
};

struct tree_iter {
    struct tree *tree;
    struct tree_node *current; 
};

struct closure {
    struct parent *closure;
    struct tree *symbols;
};

struct function {
   void (*operator)(struct function *self, struct cell); 
};

