#include "../gekkota.h"

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
    enum gka_op_lifecycle lifecycle;
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
    struct value_obj *value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
    if(op->lifecycle == GKA_OP_NOT_STARTED){
        op->lifecycle = GKA_OP_STARTED;
        return 0;
    }

    /*
    printf("in test %d\n", op->in_test);
    printf("\x1b[36mcell:");
    print_cell(ctx->cell);
    print_cell(ctx->cell->next);
    print_cell(ctx->cell->branch);
    printf("\x1b[0m\n");
    */

    if(op->done){
        while(ctx->cell){
            ctx->cell = ctx->cell->next;
        }
    }else if(value && !value->truthy(value)){
        cell_next(ctx); 
        cell_next(ctx);
        op->in_test = 1;
        return 1;
    }else if(op->in_test){
        op->done = 1;
    }
    op->in_test = !op->in_test;
    return 0;
    /*
    if(ctx->handle_state == CRW_IN_HEAD){
        default_next(ctx);
        return;
    }
    if(!op->next){
        op->next = ctx->head->cell->next;
    }

    if(!op->done){
        if(ctx->previous && ctx->previous->value && !ctx->previous->value->truthy(ctx->previous->value)){
            if(op->next && op->next->next){
                ctx->cell = op->next = op->next->next->next;
            }else{
                ctx->cell = NULL;
            }
            return;
        }else{
            op->done = 1;
            ctx->cell = op->next = op->next->next;
            ctx->head->value = ctx->cell->value;
        }       
    } else {
        ctx->cell = NULL;
    }
    */
}

struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type) {
    struct condition_operator *op = malloc(sizeof(struct condition_operator));
    op->type = type;
    op->in_test = 1;
    op->done = 0;
    op->handle = condition_handle;
    op->new = new_condition_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
