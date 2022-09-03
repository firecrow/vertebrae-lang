struct crw_state {
    enum CRW_STATUS status;
    struct head *head;
    struct cell *cell;
    struct stack_item *stack
    struct closure *closure;
    bool (*next)(struct step_state *ctx);
};

bool crw_next_step(struct step_state *ctx);
