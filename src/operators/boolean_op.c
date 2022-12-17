#include "../gekkota.h"

/* =========== boolean ==========*/
static bool boolean_handle(struct operator_ifc *op, struct crw_state *ctx){
    if(op->lifecycle != GKA_OP_STARTED){
        return 0;
    }
    if(ctx->value->truthy && !ctx->value->truthy(ctx->value)){
        ctx->head->value = ctx->builtins.true;
        return 0;
    }
    ctx->head->value = ctx->builtins.false;
}

struct operator_ifc *boolean_singleton = NULL;

struct operator_ifc *new_boolean_operator(enum OPERATOR_TYPE type) {
    if(!boolean_singleton){
        boolean_singleton = malloc(sizeof(struct operator_ifc));
        boolean_singleton->type = type;
        boolean_singleton->handle = boolean_handle;
        boolean_singleton->close = NULL;
        boolean_singleton->new = new_boolean_operator;
    }
    return boolean_singleton;
}
