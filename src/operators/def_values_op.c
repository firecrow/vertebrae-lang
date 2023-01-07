#include "../gekkota.h"

static int debug = 0;
 
static void def_value(struct crw_state *ctx, struct value_obj *key, struct value_obj *value){
    if(debug){
        printf("defining value: ");
        print_value(value);
        printf("\n");
    }

    struct closure *closure = ctx->head->closure->parent;
    tree_add(closure->symbols, key->slot.string, value);
}

static void set_value(struct crw_state *ctx, struct value_obj *key, struct value_obj *value){
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

static char def_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct def_operator *op = (struct def_operator *) _op;
    if(op->lifecycle != GKA_OP_STARTED){
        return 0;
    }

    if(op->key == NULL){
        if(ctx->value && (ctx->value->type == SL_TYPE_SYMBOL)){
            op->key = ctx->value;
        }
    }else if(!value_is_nil(ctx->value)){
        op->handle_value(ctx, op->key, ctx->value);
        op->key = NULL;
    }
    return 0;
}

struct operator_ifc * new_def_operator(enum OPERATOR_TYPE type) {
    struct def_operator *op = malloc(sizeof(struct def_operator));
    memset(op, 0, sizeof(struct def_operator));
    op->type = type;
    op->handle = def_handle;
    op->handle_value = def_value;
    op->new = new_def_operator;
    return op;
}

struct operator_ifc * new_set_operator(enum OPERATOR_TYPE type) {
    struct def_operator *op = malloc(sizeof(struct def_operator));
    memset(op, 0, sizeof(struct def_operator));
    op->type = type;
    op->handle = def_handle;
    op->handle_value = set_value;
    op->new = new_set_operator;
    return op;
}
