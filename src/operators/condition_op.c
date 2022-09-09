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
    bool in_body;
    struct cell *next;
};

/* 
 * idea is to have an alternating set of cells for run or dont run 
 * and have the op->next be the series of base cells from head to hte end
 * and have cell->next be the result of each branch
 * such that each successful branch will run it's next cell on the op->next level
 */
static void condition_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct condition_operator *op = (struct condition_operator *)_op;
    if(!op->next){
        op->next = ctx->head->cell->next;
    }
    if(!op->in_body){
        if(ctx->cell->value->type == SL_TYPE_CELL){
            ctx->cell = ctx->cell->value->slot.cell;
            op->next = op->next->next;
            op->in_body = 1;
            return;
        }
    }else{
        struct value_obj *value = ctx->cell->value;
        if(value->truthy(value)){
            if(ctx->cell->next->value->type == SL_TYPE_CELL){
                ctx->cell = ctx->cell->next->value->slot.cell;
                op->next = op->next->next;
                op->in_body = 0;
                return;
            }
        }else{
            op->next = op->next->next;
        }
    }
    ctx->cell = op->next->next;
}

struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type) {
    struct condition_operator *op = malloc(sizeof(struct condition_operator));
    op->type = type;
    op->handle = condition_handle;
    op->new = new_condition_operator;
    return (struct operator_ifc *)op;
}
