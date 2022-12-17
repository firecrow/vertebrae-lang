#include "../gekkota.h"

static int debug = 0;

enum phases {
    NOT_STARTEd = 0,
    IF,
    ELSE_IF,
    ELSE,
    DONE
};

struct condition_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
    bool done;
    bool in_test;
};

/* 
 * idea is to have an alternating set of cells for run or dont run 
 * and have the op->next be the series of base cells from head to hte end
 * and have cell->next be the result of each branch
 * such that each successful branch will run it's next cell on the op->next level
 */
static bool condition_handle(struct operator_ifc *_op, struct crw_state *ctx){
    struct condition_operator *op = (struct condition_operator *)_op;
    struct value_obj *value = swap_for_symbol(ctx->head->closure, ctx->value);

    if(debug){
        printf("\x1b[36min condition using value:");
        print_value(value);
        print_cell(ctx->cell);
        print_cell(ctx->head->cell);
        print_cell(ctx->head->cell->next);
        printf("\n\x1b[0m");
    
        printf("in test %d\n", op->in_test);
        printf("done %d\n", op->done);
        printf("\x1b[0m\n");
    }
    return 0;

    if(op->done){
        if(debug){
            printf("in done fast forwarding ctx->cell is: ");
            print_cell(ctx->cell);
            printf("\n");
        }
        while(ctx->cell){
            ctx->cell = ctx->cell->next;
        }
    }else if(value && !value->truthy(value)){
        if(debug){
            printf("not truty ctx->cell is: ");
            print_cell(ctx->cell);
            printf("\n");
        }
        cell_next(ctx); 
        if(debug){
            printf("moving to: ");
            print_cell(ctx->cell);
            printf("\n");
        }   
        op->in_test = 1;
        return 1;
    }else if(op->in_test){
        op->done = 1;
    }
    op->in_test = !op->in_test;
    return 0;
}

struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type) {
    struct condition_operator *op = malloc(sizeof(struct condition_operator));
    op->type = type;
    op->in_test = 1;
    op->done = 0;
    op->handle = condition_handle;
    op->new = new_condition_operator;
    op->lifecycle = GKA_OP_NOT_STARTED;
    return (struct operator_ifc *)op;
}
