#include "../gekkota.h"

static void passthrough(struct crw_state *ctx, struct head *previous){
    printf("\x1b[35mpassthrough\x1b[0m\n");
    struct head *head = ctx->head;
    ctx->previous = previous;
    ctx->value = previous->value;

    head->value = previous->value;
    ctx->value = head->value;

    head->operator->handle(head->operator, ctx);
    ctx->previous = NULL;
}

struct stack_item *push_stack(struct crw_state *ctx, struct cell *cell){
    if(!ctx->head){
        return NULL;
    }
    ctx->nesting++;
    struct stack_item *item = new_stack_item(ctx->stack, cell, ctx->head);
    return item;
}

void close_branch(struct crw_state *ctx){
    /*
    ctx->handle_state = CRW_IN_CLOSE;
    ctx->head->operator->handle(ctx->head->operator, ctx);
    */
}

void start_new_branch(struct crw_state *ctx, struct cell *cell, struct closure *closure){
    ctx->stack = push_stack(ctx, ctx->cell);
    print_cell(cell);
    ctx->head = setup_new_head(new_head(), cell, closure);
    ctx->cell = cell;
}

void pop_stack(struct crw_state *ctx){
    struct head *previous = ctx->head;
    ctx->cell = ctx->stack->cell;
    ctx->head = ctx->stack->head;
    ctx->stack = ctx->stack->previous;
    passthrough(ctx, previous);
    ctx->nesting--;
}

static void next_step(struct crw_state *ctx);

struct crw_state *crw_new_state_context(){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 

   if(state == NULL){
      return NULL;
   }

   memset(state, 0, sizeof(struct crw_state));
   return state;
}

void crw_setup_state_context(struct crw_state *state, struct cell* root, struct closure *global){
   state->next = next_step;

   state->builtins.true = new_result_value_obj(TRUE);
   state->builtins.false = new_result_value_obj(FALSE);
   state->builtins.nil = new_result_value_obj(NIL);
   state->builtins.error = new_result_value_obj(ERROR);

   state->status = CRW_CONTINUE;

   start_new_branch(state, root, global);
}

static void next_step(struct crw_state *ctx){
    if(ctx->cell == NULL){
        ctx->status = CRW_DONE;
        return;
    }

    ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);

    print_value(ctx->cell->value);
    printf("\x1b[0mto: \x1b[34m");
    print_value(ctx->value);
    printf("\n\x1b[0m");

    if(ctx->cell->value){
        ctx->head->operator->handle(ctx->head->operator, ctx);
    }else{
        cell_incr(ctx);
    }
    ctx->status = ctx->cell ? CRW_CONTINUE : CRW_DONE;

    printf("status %s\n", ctx->status == CRW_CONTINUE ? "CONTINUE" : "DONE");
}

void cell_incr(struct crw_state *ctx){
    if(!ctx->cell){
        return;
    }

    printf("entering incr: ");
    print_cell(ctx->cell);
    printf("\n");

    crw_process_keys(ctx);

    int is_moved = 0;
    while(ctx->cell->branch && ctx->cell->branch){
        printf("branching--->\n");
        is_moved = 1;
        start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);
    }

    if(!is_moved){
        printf("nexting--->\n");
        ctx->cell = ctx->cell->next;
    }

    if(ctx->cell == NULL){
        close_branch(ctx);
        while(ctx->cell == NULL && ctx->stack){
            pop_stack(ctx);
        }
        ctx->handle_state = CRW_IN_ARG;
        ctx->cell = ctx->cell ? ctx->cell->next : NULL;
    }

    printf("leaving incr(%d): ", ctx->handle_state);
    print_cell(ctx->cell);
    printf("\n");
}

void run_root(struct crw_state *ctx, struct cell *root){
    struct closure *global = new_closure(NULL);

    init_basic_library(global); 

    crw_setup_state_context(ctx, root, global);

    while(ctx->status != CRW_DONE){
       ctx->next(ctx); 
    }
}
