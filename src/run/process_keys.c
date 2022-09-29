#include "../gekkota.h"

bool crw_process_keys(struct crw_state *ctx){
    struct value_obj *value = ctx->value;
    if(!ctx->head){
        return 0;
    }
    /*if(value && ((value->type == SL_TYPE_KEY) || (value->type == SL_TYPE_SET_LEX))){*/
    if(value && value->type == SL_TYPE_KEY){
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->head->key_for_next = value;
        return 1;
    }else if(value && ctx->head->key_for_next){
        tree_update(ctx->head->closure->symbols, ctx->head->key_for_next->slot.string, value);
        /*
        struct value_obj *key = ctx->head->key_for_next;
        if(key->type == SL_TYPE_KEY){
            tree_update(ctx->head->closure->symbols, key->slot.string, value);
            printf("in key......\n");
        } else if(key->type == SL_TYPE_SET_LEX) {
            printf("in lex......\n");
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
        */
        ctx->head->key_for_next = NULL;
        return 1;
    }
    return 0;
}
