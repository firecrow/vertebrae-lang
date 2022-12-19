#include "../gekkota.h"

static bool save_cell_handle(struct operator_ifc *op, struct crw_state *ctx){
    if(op->lifecycle != GKA_OP_STARTED){
        return 0;
    }
    struct crw_ctx_data *data = new_data();
    data->type = SL_TYPE_CELL;
    data->slot.cell = ctx->cell;
    ctx->data = data;

    if(ctx->previous){
        ctx->head->value = ctx->previous->value;
    }
    return 0;
}

struct operator_ifc * new_save_cell_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = save_cell_handle;
    op->new = new_save_cell_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return op;
}
