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
    /* used mosstly by unit tests for now */
    struct head *context;
    struct cell *cell;
    struct stack_item *stack;
    struct value_obj *value;
    struct {
        struct value_obj *true;
        struct value_obj *false;
        struct value_obj *nil;
        struct value_obj *error;
    } builtins;
    void (*next)(struct crw_state *ctx);
    void (*default_handle)(struct operator_ifc *_op, struct crw_state *ctx); 
    struct crw_ctx_data *data;

    /* for debugging */
    int nesting;
};

struct crw_state *crw_new_state_context();
void crw_setup_state_context(struct crw_state *state, struct cell* root, struct closure *global);
struct stack_item *push_stack(struct crw_state *ctx, struct cell *cell);
void pop_stack(struct crw_state *ctx);
void close_branch(struct crw_state *ctx);
void run_root(struct crw_state *ctx, struct cell *root);
void cell_incr(struct crw_state *ctx);
void cell_next(struct crw_state *ctx);
void start_new_branch(struct crw_state *ctx, struct cell *cell, struct closure *closure);

/* utils */
struct cell *get_next(struct cell *cell);
