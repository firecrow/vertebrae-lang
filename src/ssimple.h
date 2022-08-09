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

struct string {
    char *content;
    int length;
};

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

struct symbol {
   struct string name; 
   struct cell *cell;
};

struct closure {
    struct parent *closure;
    struct tree *symbols;
};

struct function {
   void (*operator)(struct function *self, struct cell); 
};

struct cell {
    struct closure *closure;  
    struct symbol *symbol; // nullable
    struct function *function; // nullable
    struct cell *next;
};
