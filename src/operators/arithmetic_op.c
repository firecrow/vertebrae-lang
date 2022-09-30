#include "../gekkota.h"

struct arithmetic_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct value *value;
};

static void arithmetic_handle(struct operator_ifc *_op, struct crw_state *ctx){
    printf("arithamtic.........%d\n", ctx->handle_state);
    if(ctx->handle_state == CRW_IN_HEAD){
        printf("HEAD returning...\n");
        ctx->handle_state = CRW_IN_ARG;
        cell_incr(ctx);
        return;
    }
    if(ctx->handle_state == CRW_IN_CLOSE){
        cell_incr(ctx);
        return;
    }
    struct head *head = ctx->head;
    struct value_obj *value = ctx->value;
    if(!value){
        cell_incr(ctx);
        return;
    }
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Cannot do arithmetic on non integer value recieved of type %d", value->type);
        cell_incr(ctx);
        return;
        /*exit(1);*/
    }
    struct arithmetic_operator *op = (struct arithmetic_operator*)_op;
    if(!head->value){
        head->value = clone_value(value);
        cell_incr(ctx);
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

    cell_incr(ctx);
}

struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type) {
    struct arithmetic_operator *op = malloc(sizeof(struct arithmetic_operator));
    op->type = type;
    op->handle = arithmetic_handle;
    op->new = new_arithmetic_operator;
    return (struct operator_ifc *)op;
}
