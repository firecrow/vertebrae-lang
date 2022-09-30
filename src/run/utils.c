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

void cell_incr(struct crw_state *ctx){
    printf("starting in incr:\n");
    print_cell(ctx->cell);
    printf("\n");

    int is_moved = 0;
    while(ctx->cell->branch){
        printf("->> branching");
        is_moved = 1;

        start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);

        if(!(ctx->cell && is_non_head_class(ctx->cell->value))){
            ctx->handle_state = CRW_IN_HEAD;
        }
    }
    if(!is_moved){
        printf("... nexting");
        ctx->cell = ctx->cell->next;
    }
    print_cell(ctx->cell);
    printf("\n");
}

struct cell *get_next(struct cell *cell){
    if(cell){
        return cell->next;
    }
    return NULL;
}
