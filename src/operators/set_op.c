#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

struct set_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct value *value;
};

static void set_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->handle_state == CRW_IN_HEAD){
        default_next(ctx);
        return;
    }

    struct order_entry *oentry = ctx->head->closure->symbols->order;
    while(oentry){
        if(oentry->entry){
            struct closure *closure = ctx->head->closure->parent;
            struct value_obj *result = NULL;
            while(closure && !result){
                result = tree_get(closure->symbols, oentry->entry->key);
                if(result && !value_is_nil(result)){
                    tree_add(closure->symbols, oentry->entry->key, oentry->entry->content);
                    break;
                }
                closure = closure->parent;
            }
            
        }
        oentry = oentry->next;
    }

    default_next(ctx);
}

struct operator_ifc * new_set_operator(enum OPERATOR_TYPE type) {
    struct set_operator *op = malloc(sizeof(struct set_operator));
    op->type = type;
    op->handle = set_handle;
    op->new = new_set_operator;
    return (struct operator_ifc *)op;
}
