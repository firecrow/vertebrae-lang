#include "../gekkota.h"

struct function_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct cell *next;
};

static void function_handle(struct operator_ifc *_op, struct crw_state *ctx){

    /*
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> funcion called");
    print_cell(ctx->cell);
    printf("\n");
    */

    struct function_operator *op = (struct function_operator *)_op;
    if(ctx->handle_state == CRW_IN_HEAD){
        ctx->handle_state = CRW_IN_ARG;
        /*
        default_next(ctx);
        return;
        */
    }
    if(ctx->handle_state == CRW_IN_PASSTHROUGH){
        default_next(ctx);
        return;
    }
    if(ctx->handle_state == CRW_IN_CLOSE && op->next){
        default_next(ctx);
        return;
    }

    if(ctx->cell){
        /*
        printf("adding value: ");
        print_value(ctx->cell->value);
        printf("\n");
        */
        tree_add(ctx->head->closure->symbols, str("value"), ctx->cell->value);
    }

    struct cell *func = ctx->head->cell;
    if(!op->next){
        op->next = ctx->cell;
    }

    /*
    printf("in func thing............\n");
    print_cell(func);
    printf(" ->\n    ");
    print_cell(func->next);
    printf("\n");
    */

    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->head = setup_new_head(new_head(), func, ctx->head->closure);
    if(func->branch){

        ctx->stack = push_stack(ctx, func);
        ctx->head = setup_new_head(new_head(), func->branch, ctx->head->closure);
        ctx->cell = func->branch;

        ctx->handle_state = CRW_IN_HEAD;
    }else{
        ctx->cell = func->next;
    }

    /*
    printf("cell after func call");
    print_cell(ctx->cell);
    printf("\n");
    */
}

struct operator_ifc * new_function_operator(enum OPERATOR_TYPE type) {
    struct function_operator *op = malloc(sizeof(struct function_operator));
    memset(op, 0, sizeof(struct function_operator));
    op->type = type;
    op->handle = function_handle;
    op->new = new_function_operator;
    return (struct operator_ifc *)op;
}
