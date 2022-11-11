#include "../gekkota.h"

static bool default_handle(struct operator_ifc *op, struct crw_state *ctx){
    ctx->head->value = ctx->value;
    return 0;
}

struct operator_ifc * new_default_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = default_handle;
    op->new = new_default_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return op;
}
