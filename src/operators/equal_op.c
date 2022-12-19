#include "../gekkota.h"

static bool equal_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct equal_operator *op = (struct equal_operator *)_op;
    if(op->lifecycle != GKA_OP_STARTED){
        return 0;
    }
    struct head *head = ctx->head;
    if(!op->value && ctx->cell){
        head->value =  ctx->builtins.false;
        op->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
        return 0;
    }

    if(op->value->equals(op->value, swap_for_symbol(ctx->head->closure, ctx->cell->value))){
        head->value = ctx->builtins.true;
    }else{
        head->value = ctx->builtins.false;
    }
}

struct operator_ifc * new_equal_operator(enum OPERATOR_TYPE type) {
    struct equal_operator *op = malloc(sizeof(struct equal_operator));
    op->type = type;
    op->handle = equal_handle;
    op->new = new_equal_operator;
    return (struct operator_ifc *)op;
}
