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

static void function_handle(struct operator_ifc *_op, struct crw_state *ctx){
    /* this is the head cell */
    if(ctx->head->cell == ctx->cell){
        default_next(ctx);
        return;
    }
    struct cell *func =  ctx->head->cell->value->slot.cell;
    ctx->head = setup_new_head(new_head(), func, ctx->head->closure);
    ctx->cell = func->next;
}

struct operator_ifc * new_function_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    op->type = type;
    op->handle = function_handle;
    op->new = new_function_operator;
    return op;
}
