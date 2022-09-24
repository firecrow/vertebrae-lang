#include "../gekkota.h"

void context_next(struct crw_state *ctx){
    if(ctx->cell){
        ctx->cell = ctx->cell->next;
    }
}

static void context_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->previous){
        ctx->head->value = ctx->previous->value;
    }
    ctx->context = ctx->head;
    context_next(ctx);
}

struct operator_ifc * new_context_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = context_handle;
    op->new = new_context_operator;
    return op;
}
