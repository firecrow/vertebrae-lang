#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

struct equal_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct value *value;
};

static void equal_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct head *head = ctx->head;
    if(!op->value){
        head->value = true_obj;
        op->value = ctx->value;
        return;
    }

    if(op->value->equals && op->value->equals(value)){
        head->value = ctx->builtins->true;
    }else{
        head->value = ctx->builtins->false;
    }

    ctx->default_handle(op, ctx);
}

struct operator_ifc * new_equal_operator(enum OPERATOR_TYPE type) {
    struct equal_operator *op = malloc(sizeof(struct equal_operator));
    op->type = type;
    op->handle = equal_handle;
    op->new = new_equal_operator;
    return (struct operator_ifc *)op;
}
