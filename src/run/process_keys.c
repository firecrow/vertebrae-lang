#include "../gekkota.h"
 
void set_value(struct crw_state *ctx, struct value_obj *value){
    printf("\x1b[36msetting a key's value\x1b[0m");
    tree_update(ctx->head->closure->symbols, ctx->head->key_for_next->slot.string, value);
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
    ctx->head->key_for_next = NULL;
    return 1;
}

bool crw_process_keys(struct crw_state *ctx){
    printf("\x1b[36mcrw keys\x1b[0m");
    struct value_obj *value = ctx->cell->value;
    if(!ctx->head){
        printf("\x1b[36m <- no head\x1b[0m");
        return 0;
    }
    print_value(value);
    if(value && ((value->type == SL_TYPE_KEY) || (value->type == SL_TYPE_SET_LEX))){
        printf("\x1b[36msetting a key\x1b[0m");
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->head->key_for_next = value;
        printf("here\n");
        print_cell(ctx->cell);
        printf("\n");

        /* cant assign a branch, if a branch is quoted into a cell it can be assigned */
        if(ctx->cell->next && !ctx->cell->branch){
            printf("here?\n");
            set_value(ctx, ctx->cell->value);
            printf("nexting in process keys ...............\n");
            ctx->cell = ctx->cell->next;
        }
    }
}
