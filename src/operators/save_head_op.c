#include "../gekkota.h"

static void save_head_handle(struct operator_ifc *op, struct crw_state *ctx){
    printf("--------- in save head\n");
    if(ctx->previous){
        printf("--------- setting\n");

        /* saving the head to context data */
        struct crw_ctx_data *data = new_data();
        data->type = SL_TYPE_HEAD;
        data->slot.head = ctx->previous;
        ctx->data = data;

        ctx->head->value = ctx->previous->value;
    }
    if(ctx->handle_state == CRW_IN_ARG){
        cell_incr(ctx);
    }
}

struct operator_ifc * new_save_head_operator(enum OPERATOR_TYPE type) {
    struct operator_ifc *op = malloc(sizeof(struct operator_ifc));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = save_head_handle;
    op->new = new_save_head_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return op;
}
