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
    SL_TYPE_VALUE,
    SL_TYPE_COMMENT
    /* custom types */
};

enum SL_PARSE_STATE {
    START = 0,
    IN_CELL,
    IN_COMMENT,
    IN_STRING
};

#define bool char

struct string;
struct tree_node;
struct tree;
struct tree_iter;
struct closure;
struct function;
struct cell;

struct tree_node {
    void *content;
    unsigned long hash;
    struct tree_node *right;
    struct tree_node *left;
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

struct closure_entry {
    struct closure *closure;
    struct string *name;
    enum SL_TYPE type;
    union {
        struct cell *cell;
        struct value *value;
        struct operator_ifc *function;
    } body;
};

struct closure {
    struct parent *closure;
    struct tree *symbols;
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
        struct operator_ifc *operator;
        char c;
        void *custom;
    } slot;
};

typedef void (*operator_handle_func)(struct operator_ifc *_op, struct cell *head, struct cell *cell);

struct operator_ifc {
    int type;
    struct operator_ifc *(*new)();
    operator_handle_func *handle;
};

struct cell {
    int id;
    struct value_obj *value;
    struct cell *next;
    struct cell *branch;
};

struct parse_ctx {
    struct cell *current;
    struct cell *root;
    struct stack_item *stack;
    enum SL_PARSE_STATE state;
    enum SL_PARSE_STATE prev_state;
    bool in_escape;
    bool has_comment_char;
    struct string *token;
};

struct stack_item {
    struct cell *cell;
    struct head *head;
    struct stack_item *previous;
};

void print_cell(struct cell *cell);
struct string *string_from_cstring(char *cstring);

struct head {
    struct operator_ifc *operator;
    struct closure *closure;  
    struct value_obj *value;
};
