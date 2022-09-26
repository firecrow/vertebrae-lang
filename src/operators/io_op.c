#include "../gekkota.h"

/* =========== print ==========*/
struct print_operator {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

static void print_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->handle_state == CRW_IN_HEAD){
        default_next(ctx);
        ctx->handle_state = CRW_IN_ARG;
        return;
    }
    if(tree_get(ctx->head->closure->symbols, str("head")) != NULL){
        printf("\x1b[34m");
        print_head(ctx->head);
        printf("\x1b[0m");
    }else if(tree_get(ctx->head->closure->symbols, str("cell")) != NULL){
        printf("\x1b[34m");
        print_cell(ctx->cell);
        printf("\x1b[0m");
    }else if(tree_get(ctx->head->closure->symbols, str("tree")) != NULL){
        printf("\x1b[34m<tree ");
        print_tree(ctx->head->closure->symbols);
        printf(">\x1b[0m");
    }else if(tree_get(ctx->head->closure->symbols, str("data")) != NULL){
        struct closure *closure = ctx->head->closure;
        while(closure){
            printf("\x1b[34m<---- tree\n");
            print_tree(closure->symbols);
            printf("\x1b[0m\n");
            closure = closure->parent;
        }
    }else{
        /* this means it's not a finish call */
        if(ctx->value && ctx->handle_state != CRW_IN_CLOSE){
            printf("\x1b[33m%s\x1b[0m", ctx->value->to_string(ctx->value)->content);
        }else if(ctx->handle_state == CRW_IN_CLOSE){
            printf("\n");
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
