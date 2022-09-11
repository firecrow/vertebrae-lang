#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

bool crw_process_keys(struct crw_state *ctx){
    struct value_obj *value = ctx->value;
    if(!ctx->head){
        return 0;
    }
    if(value && value->type == SL_TYPE_KEY){
        tree_update(ctx->head->closure->symbols, value->slot.string, ctx->builtins.nil);
        ctx->head->key_for_next = value;
        return 1;
    }else if(value && ctx->head->key_for_next){
        tree_update(ctx->head->closure->symbols, ctx->head->key_for_next->slot.string, value);
        ctx->head->key_for_next = NULL;
    }
    return 0;
}
