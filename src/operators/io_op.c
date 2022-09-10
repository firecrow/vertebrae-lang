#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../run/run.h"
#include "operator.h"

/* =========== print ==========*/
struct print_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

static void print_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->head->cell == ctx->cell){
        return;
    }
    if(tree_get(ctx->head->closure->symbols, str("head")) != NULL){
        print_head(ctx->head);
        printf("\n");
    }else if(tree_get(ctx->head->closure->symbols, str("cell")) != NULL){
        print_cell(ctx->cell);
        printf("\n");
    }else{
        /* this means it's not a finish call */
        if(ctx->cell != NULL && ctx->value){
            printf("\x1b[34m%s\x1b[0m\n", ctx->value->to_string(ctx->value)->content);
        }
    }
    default_next(ctx);
}

struct print_operator *print_singleton;
struct operator_ifc *new_print_operator(enum OPERATOR_TYPE type) {
    if(!print_singleton){
        print_singleton = malloc(sizeof(struct print_operator));
        print_singleton->type = type;
        print_singleton->handle = print_handle;
        print_singleton->new = new_print_operator;
    }
    return (struct operator_ifc *)print_singleton;
}
