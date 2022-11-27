#include "../gekkota.h"

static int debug = 0;

struct arithmetic_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
    struct value *value;
};

static bool arithmetic_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(debug){
        printf("\x1b[32min arithmetic: ");
        print_value(ctx->value);
        printf("\x1b[0m\n");
    }
    struct arithmetic_operator *op = (struct arithmetic_operator*)_op;
    struct head *head = ctx->head;
    struct value_obj *value = ctx->value;
    if(!value){
        printf(" returning no value ");
        return 0;
    }
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Cannot do arithmetic on non integer value recieved of type %d\n", value->type);
        print_value(value);
        printf("\n");
        return 0;
    }
    if(!head->value){
        head->value = clone_value(value);
        if(debug){
            printf("\x1b[32msetting original value\n\x1b[0m");
        }
        return 0;
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
    }else if(op->type == GREATER_THAN){
        head->value = (new_value > head->value->slot.integer) ?  ctx->builtins.true :  ctx->builtins.false;
    }else if(op->type == LESS_THAN){
        head->value = (new_value < head->value->slot.integer) ?  ctx->builtins.true :  ctx->builtins.false;
    }
    return 0;
}

struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type) {
    struct arithmetic_operator *op = malloc(sizeof(struct arithmetic_operator));
    op->type = type;
    op->handle = arithmetic_handle;
    op->new = new_arithmetic_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
