#include "../gekkota.h"
 
void set_value(struct crw_state *ctx, struct value_obj *value){
    struct value_obj *key = ctx->head->key_for_next;
    if(key->type == SL_TYPE_KEY){
        tree_update(ctx->head->closure->symbols, key->slot.string, value);
    } else if(key->type == SL_TYPE_SET_LEX) {
        struct closure *closure = ctx->head->closure;
        struct value_obj *result = NULL;
        while(closure && !result){
            result = tree_get(closure->symbols, key->slot.string);
            if(result && !value_is_nil(result)){
                tree_add(closure->symbols, key->slot.string, value);
                break;
            }
            closure = closure->parent;
        }
    }
    ctx->head->key_for_next = NULL;
    return 1;
}

bool crw_process_keys(struct crw_state *ctx){
    struct value_obj *value = ctx->cell->value;
    if(!ctx->head){
        return 0;
    }
    if(value && ((value->type == SL_TYPE_KEY) || (value->type == SL_TYPE_SET_LEX))){
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->head->key_for_next = value;

        /* cant assign a branch, if a branch is quoted into a cell it can be assigned */
        if(ctx->cell->next && !ctx->cell->branch){
            set_value(ctx, ctx->cell->value);
            ctx->cell = ctx->cell->next;
        }
    }
}
