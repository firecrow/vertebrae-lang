#include "../gekkota.h"

static void default_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->previous){
        ctx->head->value = ctx->previous->value;
    }
    if(ctx->handle_state == CRW_IN_ARG){
        cell_incr(ctx);
    }
    if(ctx->handle_state == CRW_IN_HEAD){
        ctx->handle_state = CRW_IN_ARG;
    }
}

struct operator_ifc * new_default_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = default_handle;
    op->new = new_default_operator;
    return op;
}
