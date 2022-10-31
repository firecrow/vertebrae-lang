#include "../gekkota.h"

static int debug = 0;
 
static void set_value(struct crw_state *ctx, struct value_obj *value){
    if(debug){
        printf("setting value: ");
        print_value(value);
        printf("\n");
    }

    struct value_obj *key = ctx->head->key_for_next;
    if(key->type == SL_TYPE_KEY){
        tree_update(ctx->head->closure->symbols, key->slot.string, value);
    } else if(key->type == SL_TYPE_SET_LEX) {
        struct closure *closure = ctx->head->closure;
        struct closure *previous = closure;
        struct value_obj *result = NULL;
        while(closure && (!result || value_is_nil(result))){
            previous = closure;
            result = tree_get(closure->symbols, key->slot.string);
            closure = closure->parent;
        }
        if(result){
            tree_add(previous->symbols, key->slot.string, value);
        }
    }
    ctx->head->key_for_next = NULL;
    return 1;
}

static bool set_values(struct crw_state *ctx){
    struct value_obj *value = ctx->cell->value;
    if(!ctx->head){
        return 0;
    }
    if(value && (value->type == SL_TYPE_KEY)){
        if(debug){
            printf("\x1b[35mdef keys: ");
            print_cell(ctx->cell);
            printf("\x1b[0m\n");
        }
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->head->key_for_next = value;

        /* cant assign a branch, if a branch is quoted into a cell it can be assigned */
        if(ctx->cell->next && !ctx->cell->branch){
            set_value(ctx, ctx->cell->next->value);
            ctx->cell = ctx->cell->next;
        }
    }else if(value && (value->type == SL_TYPE_SET_LEX)){
        if(debug){
            printf("\x1b[35mset keys: ");
            print_cell(ctx->cell);
            printf("\x1b[0m\n");
        }
        ctx->head->key_for_next = value;

        /* cant assign a branch, if a branch is quoted into a cell it can be assigned */
        if(ctx->cell->next && !ctx->cell->branch){
            set_value(ctx, ctx->cell->next->value);
            ctx->cell = ctx->cell->next;
        }
    }else{
        if(debug){
            printf("\x1b[36mno keys: ");
            print_cell(ctx->cell);
            printf("\x1b[0m\n");
        }
    }
}
