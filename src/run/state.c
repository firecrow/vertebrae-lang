#include "../gekkota.h"

int debug = 0;
int indent = 0;

static void print_space(){
    int in = indent*4;
    while(in-- > 0){
        printf(" ");
    }
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
    if(ctx->head->operator->close){
        ctx->head->operator->close(ctx->head->operator, ctx);
    }
}

void start_new_branch(struct crw_state *ctx, struct cell *cell, struct closure *closure){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->previous = ctx->head;

    ctx->head = setup_new_head(new_head(), cell, closure);
    ctx->cell = cell;
    ctx->head->operator->handle(ctx->head->operator, ctx);
    ctx->head->operator->lifecycle = GKA_OP_STARTED;

    if(debug){
        printf("\x1b[35mstarting branch from...\x1b[0m");
        print_cell(cell);
        printf("\n");
    }
}

static void next_step(struct crw_state *ctx);

struct crw_state *crw_new_state_context(){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 
   memset(state, 0, sizeof(struct crw_state));

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
    if(debug){
        printf("next step: with cell : -->\n");
        printf("   ");
        print_cell(ctx->cell);
        printf("\n");
        printf("   ");
        print_head(ctx->head);
        printf("\n");
    }
    if(ctx->cell){
        int is_moved = 0;
        while(ctx->cell && ctx->cell->branch){
            start_new_branch(ctx, ctx->cell->branch, ctx->head->closure);
            indent++;
            return;
        }

        ctx->cell = ctx->cell->next;
    }

    if(!ctx->cell && ctx->stack){
        struct head *previous = ctx->head;
        ctx->cell = ctx->stack->cell ? ctx->stack->cell->next : NULL;
        ctx->head = ctx->stack->head;
        ctx->stack = ctx->stack->previous;
        ctx->value = swap_for_symbol(ctx->head->closure, previous->value);
        ctx->head->operator->handle(ctx->head->operator, ctx);
        if(ctx->cell && ctx->cell->branch){
            return;
        }
        ctx->nesting--;
        indent--;
    }
    if(ctx->cell){
        ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
        ctx->head->operator->handle(ctx->head->operator, ctx);
    }
    
    ctx->status = CRW_CONTINUE;
    if(!ctx->cell && !ctx->stack) ctx->status = CRW_DONE;
}

void cell_next(struct crw_state *ctx){
    ctx->cell = ctx->cell->next;
}

void run_root(struct crw_state *ctx, struct cell *root){
    if(root == NULL){
        fprintf(stderr, "Error root is NULL\n");
        exit(1);
    }
    struct closure *global = new_closure(NULL);

    init_basic_library(global); 

    crw_setup_state_context(ctx, root, global);

    while(ctx->status != CRW_DONE){
       ctx->next(ctx); 
    }
}
