#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

bool crw_process_keys(struct crw_state *ctx){
    struct head *head = ctx->head;
    struct value_obj *value = ctx->cell->value;
    if(value && value->type == SL_TYPE_KEY){
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->key_for_next = value;
        return 1;
    }else if(ctx->key_for_next){
        tree_update(ctx->head->closure->symbols, ctx->key_for_next->slot.string, value);
        ctx->key_for_next = NULL;
        return 0;
    }else{
        return 0;
    }
}
