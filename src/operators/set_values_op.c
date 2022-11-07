#include "../gekkota.h"

static int debug = 0;

struct print_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
};
 
static void set_value(struct crw_state *ctx, struct value_obj *value, struct value_obj *key){
    if(debug){
        printf("setting value: ");
        print_value(value);
        printf("\n");
    }

    struct closure *closure = ctx->head->closure;
    struct closure *previous = closure;
    struct value_obj *result = NULL;

    while(closure && !result){
        previous = closure;
        result = tree_get(closure->symbols, key->slot.string);
        closure = closure->parent;
    }

    if(result){
        tree_add(previous->symbols, key->slot.string, value);
    }
}

static char set_handle(struct operator_ifc *_op, struct crw_state *ctx){
    set_value(ctx, ctx->head->cell->value, ctx->cell->value);
    return 0;
}

struct operator_ifc * new_set_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = set_handle;
    op->new = new_set_operator;
    return op;
}
