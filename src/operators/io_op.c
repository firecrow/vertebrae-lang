#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "operator.h"

/* =========== print ==========*/
struct print_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

static void print_handle(struct operator_ifc *_op, struct head *head, struct value_obj *value){
    if(value->to_string){
        printf("%s", value->to_string(value)->content);
    }else{
        fprintf(stderr, "Cannot print non string value");
        exit(1);
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