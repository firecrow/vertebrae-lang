#include "../gekkota.h"

struct save_count_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
    int count;
};

static bool save_count_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct save_count_operator *op = (struct save_count_operator *)_op;
    op->count++;

    if(ctx->previous){
        ctx->head->value = ctx->previous->value;
    }

    struct crw_ctx_data *data = new_data();
    data->type = SL_TYPE_VALUE;
    data->slot.head =  new_int_value_obj(op->count);
    ctx->data = data;
    return 0;
}

struct operator_ifc * new_save_count_operator(enum OPERATOR_TYPE type) {
    struct save_count_operator *op = malloc(sizeof(struct save_count_operator));
    memset(op, 0, sizeof(struct operator_ifc));
    op->type = type;
    op->handle = save_count_handle;
    op->new = new_save_count_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    op->count = 0;
    return op;
}
