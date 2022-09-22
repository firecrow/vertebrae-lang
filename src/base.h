#define STDIN 0 
#define STDOUT 1
#define STDERR 2 

#define BUFFER_CHUNK_SIZE 256

#define STRING_DEFAULT_SIZE 4

#define max(AARG, BARG) \
    (AARG) > (BARG) ? (AARG) : (BARG)

#define min(AARG, BARG) \
    (AARG) < (BARG) ? (AARG) : (BARG)

enum CRW_RESULT {
    FALSE = 0,
    TRUE,
    NIL,
    ERROR
};

enum SL_TYPE {
    SL_TYPE_NONE = 0,
    SL_TYPE_BOOLEAN_RESULT,
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
    SL_TYPE_COMMENT,
    SL_TYPE_QUOTE,
    SL_TYPE_KEY
    /* custom types */
};

enum parse_accent {
  GKA_PARSE_NO_ACCENT,
  GKA_PARSE_QUOTE,
  GKA_PARSE_SUPER,
  GKA_PARSE_DOT,
  GKA_PARSE_NOT
};

#define bool char

struct string;
struct tree;
struct tree_entry;
struct closure;
struct closure_entry;
struct function;
struct cell;
struct head;

struct closure_entry {
    struct closure *closure;
    struct string *name;
    enum SL_TYPE type;
    union {
        struct cell *cell;
        struct value_obj *value;
        struct operator_ifc *function;
    } body;
};

struct closure {
    struct closure *parent;
    struct tree *symbols;
    struct value_obj* (*lookup)(struct closure *closure, struct value_obj *value);
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
        struct cell *cell;
        struct operator_ifc *operator;
        char c;
        void *custom;
        enum CRW_RESULT result;
    } slot;
    struct string *(*to_string)(struct value_obj *value);
    bool (*truthy)(struct value_obj *value);
    bool (*equals)(struct value_obj *source, struct value_obj *compare);
};

struct cell {
    int id;
    struct value_obj *value;
    enum parse_accent accent;
    struct cell *next;
    struct cell *branch;
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
    struct value_obj *source;
    struct cell *cell;
    struct value_obj *key_for_next;
};

struct value_obj *swap_for_symbol(struct closure *closure, struct value_obj *value);

