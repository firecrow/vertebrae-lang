/* cell.c */
struct value_obj *get_or_create_value(struct cell *cell);
struct cell *new_cell();
bool is_branch_cell(struct cell *cell);

/* closure.c */
struct closure *new_closure(struct closure *parent);
struct closure_entry *new_closure_entry(struct closure *closure, bool is_function);
void closure_add_cell(struct closure *closure, struct string *key, struct cell *cell);
void closure_add_value(struct closure *closure, struct string *key, struct value *value);
void closure_add_function(struct closure *closure, struct string *key, struct operator_ifc *op);
struct value_obj *closure_lookup(struct closure *closure, struct value_obj *value);

/* debug.c */
void print_value(struct value_obj *value);
void print_cell(struct cell *cell);
void print_head(struct head *head);
void print_tree(struct tree *tree);

/* head.c */
struct head *new_head();
void setup_head(struct head *head, struct cell *branch, struct head *current_head);

/* parse.c */
struct parse_ctx *new_parse_ctx();
struct cell *parse_all(struct string *string);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);

/* stack.c */
struct stack_item *new_stack_item(struct stack_item *existing, struct cell *cell, struct head *head);
struct stack_item *push_stack(struct stack_item *existing, struct cell *cell, struct head *head);
