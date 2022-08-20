#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "operator.h"

struct arithmetic_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct value *value;
};

void arithmetic_handle(struct operator_ifc *_op, struct head *head, struct value_obj *value){
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Cannot do arithmetic on non integer value");
        exit(1);
    }
    struct arithmetic_operator *op = (struct arithmetic_operator*)_op;
    if(!head->value){
        head->value = clone_value(value);
        return;
    }

    int new_value = value->slot.integer;

    if(op->type == ADD){
        printf("ADDITINON\n");
        head->value->slot.integer += new_value; 
    }else if(op->type == SUBTRACT){
        printf("SUB\n");
        head->value->slot.integer -= new_value;
    }else if(op->type == DIVIDE){
        head->value->slot.integer = head->value->slot.integer / new_value;
    }else if(op->type == MULTIPLY){
        head->value->slot.integer = head->value->slot.integer * new_value;
    }
}

struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type) {
    struct arithmetic_operator *op = malloc(sizeof(struct arithmetic_operator));
    op->type = type;
    op->handle = arithmetic_handle;
    op->new = new_arithmetic_operator;
    return (struct operator_ifc *)op;
}
