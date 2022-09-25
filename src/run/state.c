#include "../gekkota.h"

static void set_custom_cell_as_head(struct crw_state *ctx, struct cell *cell){
    ctx->cell = cell;
    ctx->head = setup_new_head(new_head(), ctx->cell, ctx->head->closure);
    ctx->head->closure = ctx->head->closure;
}

static void passthrough(struct crw_state *ctx, struct head *previous){
    struct head *head = ctx->head;
    ctx->previous = previous;
    ctx->value = previous->value;

    head->value = previous->value;
    ctx->value = head->value;
    crw_process_keys(ctx);

    ctx->handle_state = CRW_IN_PASSTHROUGH;
    head->operator->handle(head->operator, ctx);
    ctx->handle_state = CRW_IN_ARG;
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
    ctx->handle_state = CRW_IN_ARG;
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

   state->handle_state = CRW_IN_ARG;
}

void start_new_branch(struct crw_state *ctx, struct cell *cell, struct closure *closure){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->head = setup_new_head(new_head(), cell, closure);
    ctx->cell = cell;
}

static void next_step(struct crw_state *ctx){
    if(!ctx->cell){
        fprintf(stderr, "Error next_step called on empty cell\n");
        exit(1);
    }

    printf("next_step cell: ");
    print_cell(ctx->cell);
    printf("\n");

    ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
    bool is_key = crw_process_keys(ctx);

    if(ctx->cell->branch){
        start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);
        if(ctx->cell && is_non_head_class(ctx->cell->value)){
            return;
        }
        ctx->handle_state = CRW_IN_HEAD;
    }

    ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);

    if(!is_key){
        ctx->head->operator->handle(ctx->head->operator, ctx);
    }else{
        default_next(ctx);  
    }

    ctx->handle_state = CRW_IN_ARG;

    if(ctx->cell == NULL){
        close_branch(ctx);

        while(ctx->cell == NULL && ctx->stack){
            pop_stack(ctx);
        }
    }
    if(!ctx->cell){
        ctx->status = CRW_DONE;
        return;
    }
    ctx->status = CRW_CONTINUE;
}

void run_root(struct crw_state *ctx, struct cell *root){
    struct closure *global = new_closure(NULL);

    init_basic_library(global); 

    crw_setup_state_context(ctx, root, global);

    while(ctx->status != CRW_DONE){
       ctx->next(ctx); 
    }
}
