enum CRW_STATUS {
    CRW_CONTINUE = 0,
    CRW_DONE = 1
};

struct crw_state {
    enum CRW_STATUS status;
    struct head *head;
    struct cell *cell;
    struct stack_item *stack;
    struct closure *closure;
    struct value_obj *key_for_next;
    bool (*next)(struct crw_state *ctx);
    /* for debugging */
    int nesting;
};

struct crw_state *crw_new_state_context(struct cell* root, struct closure *closure, struct stack_item *stack);
