#include "../gekkota.h"

static void passthrough(struct crw_state *ctx, struct head *previous){
    struct head *head = ctx->head;
    ctx->previous = previous;
    ctx->value = previous->value;

    head->value = previous->value;
    ctx->value = head->value;
    crw_process_keys(ctx);

    ctx->handle_state = CRW_IN_PASSTHROUGH;
    head->operator->handle(head->operator, ctx);
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
    ctx->handle_state = CRW_IN_CLOSE;
    ctx->head->operator->handle(ctx->head->operator, ctx);
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
   state->builtins.true = new_result_value_obj(TRUE);
   state->builtins.false = new_result_value_obj(FALSE);
   state->builtins.nil = new_result_value_obj(NIL);
   state->builtins.error = new_result_value_obj(ERROR);

   state->head = setup_new_head(new_head(), root, global);
   state->cell = root;
   state->status = CRW_CONTINUE;
   state->next = next_step;
}

static void next_step(struct crw_state *ctx){
    ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
    ctx->head->operator->handle(ctx->head->operator, ctx);
    ctx->status = ctx->cell ? CRW_CONTINUE : CRW_DONE;
}

void cell_incr(struct crw_state *ctx){
    printf("entering incr: ");
    print_cell(ctx->cell);
    printf("\n");
    if(!ctx->cell){
        fprintf(stderr, "Error next_step called on empty cell\n");
        exit(1);
    }

    int is_moved = 0;
    while(ctx->cell->branch){
        printf("->> branching\n");
        is_moved = 1;
        start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);
        if(!(ctx->cell && is_non_head_class(ctx->cell->value))){
            ctx->handle_state = CRW_IN_HEAD;
        }
    }

    if(!is_moved){
        printf("... nexting\n");
        ctx->cell = ctx->cell->next;
    }

    if(ctx->cell == NULL){
        close_branch(ctx);
        while(ctx->cell == NULL && ctx->stack){
            printf("<<< popping\n");
            pop_stack(ctx);
        }
        printf("::: landing\n");
        ctx->handle_state = CRW_IN_ARG;
        ctx->cell = ctx->cell ? ctx->cell->next : NULL;
    }
    printf("leaving incr: ");
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
