#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "operator.h"

/* =========== closure ========== */
struct closure_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

static enum SL_BRANCH_TYPE closure_handle(struct operator_ifc *_op, struct head *head, struct value_obj *value){
    head->value = value;
    return SL_CONTINUE;
}

struct operator_ifc * new_closure_operator(enum OPERATOR_TYPE type) {
    struct closure_operator *op = malloc(sizeof(struct closure_operator));
    op->type = type;
    op->handle = closure_handle;
    op->new = new_closure_operator;
    return (struct operator_ifc *)op;
}
