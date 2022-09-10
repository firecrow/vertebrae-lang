#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

static set_custom_cell_as_head(struct crw_state *ctx, struct cell *cell){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->cell = cell;
    ctx->head = setup_new_head(new_head(), ctx->cell, ctx->head->closure);
    ctx->head->closure = ctx->head->closure;
}

static bool set_cell_func(struct crw_state *ctx){
    if(ctx->head->source && ctx->head->source->type == SL_TYPE_CELL){
        set_custom_cell_as_head(ctx, ctx->head->source->slot.cell);
        ctx->head->source = NULL;
        return 1;
    }
    if(ctx->head->operator){
        ctx->head->operator->handle(ctx->head->operator, ctx);
    }
    return 0;
}

void close_branch(struct crw_state *ctx){
    ctx->value = NULL;
    ctx->head->operator->handle(ctx->head->operator, ctx);
}

void default_next(struct crw_state *ctx){
    if(ctx->cell){
        ctx->cell = ctx->cell->next;
    }
}

static void default_handle(struct operator_ifc *_op, struct crw_state *ctx){
    default_next(ctx);
}

struct operator_ifc * new_default_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = default_handle;
    op->new = new_default_operator;
    return op;
}
