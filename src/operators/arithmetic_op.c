#include "../gekkota.h"

struct arithmetic_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    enum gka_op_lifecycle lifecycle;
    struct value *value;
};

static void arithmetic_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct arithmetic_operator *op = (struct arithmetic_operator*)_op;
    if(op->lifecycle == GKA_OP_NOT_STARTED){
        op->lifecycle = GKA_OP_STARTED;
        return;
    }
    struct head *head = ctx->head;
    struct value_obj *value = ctx->value;
    if(!value){
        printf(" returning no value ");
        return;
    }
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Cannot do arithmetic on non integer value recieved of type %d ", value->type);
        print_value(value);
        printf("\n");
        return;
    }
    if(!head->value){
        head->value = clone_value(value);
        return;
    }

    int new_value = value->slot.integer;

    if(op->type == ADD){
        head->value->slot.integer += new_value; 
    }else if(op->type == SUBTRACT){
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
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
