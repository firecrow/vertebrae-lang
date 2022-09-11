#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

static enum phases {
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
    bool done;
    struct cell *next;
};

/* 
 * idea is to have an alternating set of cells for run or dont run 
 * and have the op->next be the series of base cells from head to hte end
 * and have cell->next be the result of each branch
 * such that each successful branch will run it's next cell on the op->next level
 */
static void condition_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->handle_state == CRW_IN_HEAD){
        default_next(ctx);
        return;
    }
    struct condition_operator *op = (struct condition_operator *)_op;
    if(!op->next){
        op->next = ctx->head->cell->next;
    }

    if(!op->done){
        /* skip if not truthy */
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
}

struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type) {
    struct condition_operator *op = malloc(sizeof(struct condition_operator));
    op->type = type;
    op->handle = condition_handle;
    op->new = new_condition_operator;
    return (struct operator_ifc *)op;
}
