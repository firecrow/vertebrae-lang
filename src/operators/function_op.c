#include "../gekkota.h"

static int debug = 0; 

struct function_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
    struct cell *next;
};

static bool function_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct function_operator *op = (struct function_operator *)_op;

    if(debug){
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> function called value/next\n");
        print_value(ctx->value);
        printf("\n");
        print_cell(op->next);
        printf("\n---\n");
    }

    if(!ctx->cell){
        return 0;
    }

    if(!op->next){
        op->next = ctx->cell;
    }else{
        op->next = op->next->next;
    }

    tree_add(ctx->head->closure->symbols, str("value"), ctx->value);

    ctx->stack = push_stack(ctx, op->next);
    struct cell *func = ctx->head->cell;
    ctx->head = setup_new_head(new_head(), func, ctx->head->closure);
    ctx->head = setup_new_head(new_head(), op->next, ctx->head->closure);
    ctx->cell = func;

    if(debug){
        printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< function called func/next/branches\n");
        print_cell(func);
        printf("\n");
        print_cell(op->next);
        printf("\n---\n");
        print_branches(ctx->cell, 0);
        printf("---\n");
    }


    return 0;
}

static bool function_close(struct operator_ifc *_op, struct crw_state *ctx){
    struct function_operator *op = (struct function_operator *)_op;

    if(!op->next){
        struct cell *func = ctx->head->cell;
        ctx->head = setup_new_head(new_head(), func, ctx->head->closure);
        ctx->cell = func;
        return 1;
    }
    return 0;
}

struct operator_ifc * new_function_operator(enum OPERATOR_TYPE type) {
    struct function_operator *op = malloc(sizeof(struct function_operator));
    memset(op, 0, sizeof(struct function_operator));
    op->type = type;
    op->handle = function_handle;
    op->close = function_close;
    op->new = new_function_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
