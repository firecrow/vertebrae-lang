/* string */
struct string *new_string();
struct string *clone_string(struct string *string);
struct string *string_free(struct string *string);
int _string_resize(struct string *string, size_t length);
int string_append(struct string *string, struct string *additional);
int string_append_char(struct string *string, char c);
struct string *string_from_cstring(char *cstring);
struct string *new_string_xprintf(const char *restrict format, ...);
#define str(XARG) string_from_cstring((XARG))

/* regex */
bool regex_match(char *pattern, struct string *source);

/* value_obj */
struct value_obj *new_value();
bool is_type(struct value_obj *value, enum SL_TYPE type);
struct value_obj *clone_value(struct value_obj *value);
struct value_obj *value_from_token(enum SL_PARSE_STATE state, struct string *token);
struct value_obj *new_string_value_obj(struct string *string);
struct value_obj *new_symbol_value_obj(struct string *string);
struct value_obj *new_key_value_obj(struct string *string);
struct value_obj *new_int_value_obj(int intval);
struct value_obj *new_cell_value_obj(struct cell *cell);
struct value_obj *new_result_value_obj(enum CRW_RESULT result);
bool is_string_class(struct value_obj *value);
bool is_non_head_class(struct value_obj *value);

/* tree */
struct order_entry {
    struct tree_entry *entry;
    struct order_entry *next;
    struct order_entry *previous;
    int nth_inserted;
};

struct tree_entry {
    struct value_obj *content;
    unsigned long hash;
    struct string *key;
    struct tree_entry *right;
    struct tree_entry *left;
};
struct tree {
    enum SL_TYPE type;
    int id;
    struct tree_entry *root; 
    struct order_entry *order; 
    struct order_entry *_last_order; 
    int count;
};
struct tree *new_tree();
struct value_obj *tree_get(struct tree *tree, struct string *key);
void tree_add(struct tree *tree, struct string *key, struct value_obj *value);
void tree_update(struct tree *tree, struct string *key, struct value_obj *value);
