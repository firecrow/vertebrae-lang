#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "operator.h"

/* =========== boolean ==========*/
struct boolean_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    bool condition_met;
};

static void boolean_handle(struct operator_ifc *op, struct head *head, struct value_obj *value){
    if(op->condition_met){
        return SL_BRANCH_SKIP;
    }
    if(value->truthy){
        if(value->truthy(value)){
            op->condition_met = true;
        }
    }
}

struct print_operator *print_singleton;
struct operator_ifc *new_print_operator(enum OPERATOR_TYPE type) {
    if(!print_singleton){
        print_singleton = malloc(sizeof(struct print_operator));
        print_singleton->type = type;
        print_singleton->handle = print_handle;
        print_singleton->new = new_print_operator;
    }
    return (struct operator_ifc *)print_singleton;
}
