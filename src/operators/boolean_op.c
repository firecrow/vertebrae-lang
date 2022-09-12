#include "../gekkota.h"

/* =========== boolean ==========*/
struct boolean_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    bool condition_met;
};

static void boolean_handle(struct operator_ifc *op, struct head *head, struct value_obj *value){
    if(value->truthy && !value->truthy(value)){
       return SL_BRANCH_SKIP_NEXT;
    }
    return SL_CONTINUE;
}

struct operator_ifc *new_print_operator(enum OPERATOR_TYPE type) {
    return (struct operator_ifc *)print_singleton;
}
