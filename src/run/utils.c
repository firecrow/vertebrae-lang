#include "../gekkota.h"

void default_next(struct crw_state *ctx){
    if(ctx->cell){
        ctx->cell = ctx->cell->next;
    }
}

void start_new_branch(struct crw_state *ctx, struct cell *cell, struct closure *closure){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->head = setup_new_head(new_head(), cell, closure);
    ctx->cell = cell;
}

bool cell_incr(struct crw_state *ctx, bool in_key){
    while(ctx->cell->branch){
        in_key = 0;

        start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);
        if(ctx->cell && is_non_head_class(ctx->cell->value)){
            return 1;
        }
        ctx->handle_state = CRW_IN_HEAD;

    }
    return 0;
}

struct cell *get_next(struct cell *cell){
    if(cell){
        return cell->next;
    }
    return NULL;
}
