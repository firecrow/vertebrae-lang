/* cell.c */
struct value_obj *get_or_create_value(struct cell *cell);
struct cell *new_cell(struct value_obj *value);
bool is_branch_cell(struct cell *cell);

/* closure.c */
struct closure *new_closure(struct closure *parent);
struct closure_entry *new_closure_entry(struct closure *closure, bool is_function);
void closure_add_cell(struct closure *closure, struct string *key, struct cell *cell);
void closure_add_value(struct closure *closure, struct string *key, struct value_obj *value);
void closure_add_function(struct closure *closure, struct string *key, struct operator_ifc *op);
struct value_obj *closure_lookup(struct closure *closure, struct value_obj *value);

/* debug.c */
void print_value(struct value_obj *value);
void print_cell(struct cell *cell);
void print_head(struct head *head);
void print_tree(struct tree *tree);
void print_state(struct crw_state *ctx, bool show_tree);
void print_branches(struct cell *cell, int indent);

/* head.c */
struct head *new_head();
struct head *setup_new_head(struct head *head, struct cell *branch, struct closure *closure);

/* stack.c */
struct stack_item *new_stack_item(struct stack_item *existing, struct cell *cell, struct head *head);
