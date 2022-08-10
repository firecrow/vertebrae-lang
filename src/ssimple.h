#define STDIN 0 
#define STDOUT 1
#define STDERR 2 

#define BUFFER_CHUNK_SIZE 256

#define STRING_DEFAULT_SIZE 4

enum SL_TYPE {
    SL_TYPE_NONE = 0,
    SL_TYPE_SYMBOL,
    SL_TYPE_INT,
    SL_TYPE_FLOAT,
    SL_TYPE_CHAR,
    SL_TYPE_STRING,
    SL_TYPE_TREE,
    SL_TYPE_CELL,
    SL_TYPE_CLOSURE,
    SL_TYPE_FUNCTION,
    /* custom types */
};

#define bool char

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
    enum SL_TYPE type;
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

struct string {
    char *content;
    size_t length;
    size_t allocated;
};

struct value_obj {
    int id;
    enum SL_TYPE type;
    union {
        float float_value;
        int integer;
        struct string *string;
        char c;
        void *custom;
    } value;
};

struct cell {
    int id;
    struct closure *closure;  
    struct symbol *symbol; // nullable
    struct function *function; // nullable
    struct value_obj *value;
    struct cell *next;
    struct cell *branch;
};



struct string *string_from_cstring(char *cstring);
