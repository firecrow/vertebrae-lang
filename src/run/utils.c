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

struct cell *get_next(struct cell *cell){
    if(cell){
        return cell->next;
    }
    return NULL;
}
