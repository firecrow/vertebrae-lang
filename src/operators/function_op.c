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

    if(ctx->cell && (ctx->cell->value->type == SL_TYPE_KEY || ctx->cell->value->type == SL_TYPE_SET_LEX)){
        crw_process_keys(ctx);
    }

    if(!ctx->cell){
        return 0;
    }

    if(debug){
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> funcion called\n");
        print_cell(ctx->cell);
        printf("\n");
        print_head(ctx->head);
        printf("\n");
    }

    tree_add(ctx->head->closure->symbols, str("value"), ctx->cell->value);
    struct cell *func = ctx->head->cell;

    if(!op->next){
        op->next = ctx->cell;
    }else if(op->next != NULL){
        op->next = op->next->next;
    }

    if(op->next == NULL){
        return 1;
    }

    if(debug){
        printf(">>>>>>> op->next: ");
        print_cell(op->next);
        printf("\n");

        printf("in func thing............\n");
        print_cell(func);
        printf(" ->\n    ");
        print_cell(func->branch);
        printf("\n");
    }

    /*
    printf("\x1b[36mvalue befor branch: ");
    print_value(ctx->cell->value);
    printf("\n\x1b[0m");
    */

    ctx->stack = push_stack(ctx, op->next);
    ctx->head = setup_new_head(new_head(), func->branch, ctx->head->closure);
    ctx->cell = func->branch->next;

    return 1;
}

static bool function_close(struct operator_ifc *_op, struct crw_state *ctx){
    struct function_operator *op = (struct function_operator *)_op;

    if(!op->next){
        struct cell *func = ctx->head->cell;
        ctx->stack = push_stack(ctx, NULL);
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
