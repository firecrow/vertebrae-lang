#include "../gekkota.h"

static int debug = 1;

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
    bool done;
    bool in_test;
};

/* 
 * idea is to have an alternating set of cells for run or dont run 
 * and have the op->next be the series of base cells from head to hte end
 * and have cell->next be the result of each branch
 * such that each successful branch will run it's next cell on the op->next level
 */
static bool condition_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct condition_operator *op = (struct condition_operator *)_op;
    if(op->lifecycle != GKA_OP_STARTED){
        return 0;
    }

    struct cell *root = ctx->head->cell->branch->branch;
    struct cell *current = root;

    printf("\x1b[34min condition\n");
    if(!op->done){
        if(op->in_test){
            ctx->cell = root->branch;
        }
        op->done = 1;
    }else{
        ctx->cell = ctx->head->cell->next;
    }

    /*
        while(current){
            printf("\x1b[33myes\x1b[34m\n");
            print_branches(current->branch, 0);
            current = current->next;
        }
        printf("\n\x1b[0m");
    */

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
