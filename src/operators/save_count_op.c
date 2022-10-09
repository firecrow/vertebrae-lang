#include "../gekkota.h"

static int debug = 0;

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
    
    if(debug){
        printf("count incr....");
        print_cell(ctx->cell);
        printf("\n");
    }


    struct crw_ctx_data *data = new_data();
    data->type = SL_TYPE_VALUE;
    data->slot.head =  new_int_value_obj(op->count);
    ctx->data = data;
    return 0;
}


struct operator_ifc * new_save_count_operator(enum OPERATOR_TYPE type) {
    if(!count_op){
        count_op = malloc(sizeof(struct save_count_operator));
        memset(count_op, 0, sizeof(struct operator_ifc));
    }
    count_op->type = type;
    count_op->handle = save_count_handle;
    count_op->new = new_save_count_operator;
    count_op->lifecycle = GKA_OP_NOT_STARTED;
    return count_op;
}
