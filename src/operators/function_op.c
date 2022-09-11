#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

struct function_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    /* used to determine if this is the result of a cell or result of a call */
    bool in_call;
    struct cell *next;
};

static struct function_operator *op = NULL;

static void function_handle(struct operator_ifc *_op, struct crw_state *ctx){
    /*struct function_operator *op = (struct function_operator *)_op;*/
    /* this is the head cell */
    if(ctx->head->cell == ctx->cell){
        default_next(ctx);
        return;
    }

    if(op->in_call){
        op->in_call = 0;
        return;
    }
    op->in_call = 1;

    if(ctx->cell){
        tree_add(ctx->head->closure->symbols, str("value"), ctx->cell->value);
    }

    struct cell *func = ctx->head->cell->value->slot.cell;
    if(!op->next){
        op->next = ctx->head->cell->next;
    }

    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->head = setup_new_head(new_head(), func, ctx->head->closure);
    ctx->cell = func->next;
}


struct operator_ifc * new_function_operator(enum OPERATOR_TYPE type) {
    if(op == NULL){
        op = malloc(sizeof(struct function_operator));
    }
    op->type = type;
    op->handle = function_handle;
    op->new = new_function_operator;
    return (struct operator_ifc *)op;
}
