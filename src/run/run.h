enum CRW_STATUS {
    CRW_CONTINUE = 0,
    CRW_DONE = 1
};

enum CRW_HANDLE_STATE {
   CRW_IN_ARG = 0,
   CRW_IN_HEAD,
   CRW_IN_CLOSE,
   CRW_IN_PASSTHROUGH
};

struct crw_state {
    enum CRW_STATUS status;
    enum CRW_HANDLE_STATE handle_state;
    struct head *head;
    struct head *previous;
    struct cell *cell;
    struct stack_item *stack;
    struct value_obj *key_for_next;
    struct value_obj *value;
    struct {
        struct value_obj *true;
        struct value_obj *false;
        struct value_obj *nil;
        struct value_obj *error;
    } builtins;
    bool (*next)(struct crw_state *ctx);
    void (*default_handle)(struct operator_ifc *_op, struct crw_state *ctx); 
    /* for debugging */
    int nesting;
};

struct crw_state *crw_new_state_context(struct cell* root, struct stack_item *stack);
bool crw_process_keys(struct crw_state *ctx);
struct stack_item *push_stack(struct crw_state *ctx, struct cell *cell);
void pop_stack(struct crw_state *ctx);
void close_branch(struct crw_state *ctx);

/* utils */
struct cell *get_next(struct cell *cell);
