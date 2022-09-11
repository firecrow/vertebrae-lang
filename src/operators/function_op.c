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
};

static struct function_operator *op = NULL;

static void function_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->handle_state == CRW_IN_HEAD || ctx->handle_state == CRW_IN_PASSTHROUGH){
        default_next(ctx);
        return;
    }

    if(ctx->cell != NULL){
        tree_add(ctx->head->closure->symbols, str("value"), ctx->cell->value);
    }

    struct cell *func = ctx->head->cell->value->slot.cell;

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
