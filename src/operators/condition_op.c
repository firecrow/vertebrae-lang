#include "../gekkota.h"

static int debug = 0;

enum phases {
    NOT_STARTEd = 0,
    IF,
    ELSE_IF,
    ELSE,
    DONE
};

struct condition_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
    struct cell *root;
    struct cell *stack_cell;
    bool done;
    bool in_test;
};

void next_or_jump(struct condition_operator *op, struct crw_state *ctx){
    if(!op->stack_cell || !op->stack_cell->next){
        ctx->cell = op->root->next;
    }else{
        op->stack_cell = op->stack_cell->next;
        ctx->cell = op->stack_cell;
    }
}

/* 
 * idea is to have an alternating set of cells for run or dont run 
 * and have the op->next be the series of base cells from head to hte end
 * and have cell->next be the result of each branch
 * such that each successful branch will run it's next cell on the op->next level
 */
static bool condition_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct condition_operator *op = (struct condition_operator *)_op;
    if(op->lifecycle == GKA_OP_NOT_STARTED){
        op->root = ctx->head->cell;
        op->stack_cell = op->root->branch;
        ctx->cell = op->stack_cell;

        /*
        printf("\x1b[33mstarting from: ");
        print_cell(ctx->cell);
        printf("\x1b[0m\n");
        */

        op->done = 0;
        return 0;
    }
    if(debug){
        printf("\x1b[34mvalue: ");
        print_cell(ctx->cell);
        print_value(ctx->value);
        printf("\x1b[0m\n");
    }

    if(op->done || !op->stack_cell){
        ctx->cell = op->root->next;
        return 0;
    }

    if(ctx->value && ctx->value->truthy && ctx->value->truthy(ctx->value)){
        op->done = 1;
    }else{
        next_or_jump(op, ctx);
        next_or_jump(op, ctx);
    }

    if(1 || debug){
        printf("\x1b[33mending on: ");
        print_cell(ctx->cell);
        printf("\x1b[0m\n");
    }

    return 0;
}

struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type) {
    struct condition_operator *op = malloc(sizeof(struct condition_operator));
    memset(op, 0, sizeof(struct condition_operator));
    op->type = type;
    op->in_test = 1;
    op->done = 0;
    op->handle = condition_handle;
    op->new = new_condition_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
