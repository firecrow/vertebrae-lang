#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

static set_custom_cell_as_head(struct crw_state *ctx, struct cell *cell){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->cell = cell;
    ctx->head = setup_new_head(new_head(), ctx->cell, ctx->head->closure);
    ctx->head->closure = ctx->head->closure;
}

static void passthrough(struct crw_state *ctx, struct head *previous){
    struct head *head = ctx->head;
    ctx->previous = previous;
    ctx->value = previous->value;
    ctx->handle_state = CRW_IN_PASSTHROUGH;
    head->operator->handle(head->operator, ctx);
    ctx->handle_state = CRW_IN_ARG;
    head->value = previous->value;
}

struct stack_item *push_stack(struct crw_state *ctx, struct cell *cell){
    ctx->nesting++;
    struct stack_item *item = new_stack_item(ctx->stack, cell, ctx->head);
    return item;
}

static bool set_cell_func(struct crw_state *ctx){
    if(ctx->head->source && ctx->head->source->type == SL_TYPE_CELL){
        set_custom_cell_as_head(ctx, ctx->head->source->slot.cell);
        ctx->head->source = NULL;
        return 1;
    }
    if(ctx->head->operator){
        ctx->head->operator->handle(ctx->head->operator, ctx);
    }
    return 0;
}

void close_branch(struct crw_state *ctx){
    ctx->key_for_next = NULL;
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


struct crw_state *crw_new_state_context(struct cell* root, struct stack_item *stack){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 

   if(state == NULL){
      return NULL;
   }

   memset(state, 0, sizeof(struct crw_state));

   state->builtins.true = new_result_value_obj(TRUE);
   state->builtins.false = new_result_value_obj(FALSE);
   state->builtins.nil = new_result_value_obj(NIL);
   state->builtins.error = new_result_value_obj(ERROR);

   state->head = setup_new_head(new_head(), root, stack->head->closure);
   state->cell = root;
   state->stack = stack;
   state->status = CRW_CONTINUE;
   state->next = next_step;

   state->handle_state = CRW_IN_ARG;

   return state;
}

static void next_step(struct crw_state *ctx){
    if(!ctx->cell){
        fprintf(stderr, "Error next_step called on empty cell\n");
        exit(1);
    }

    if(ctx->cell->branch){
        ctx->stack = push_stack(ctx, ctx->cell);
        printf("\x1b[32mbranch %d", ctx->head->closure->symbols->id);
        print_cell(ctx->cell->branch);
        ctx->head = setup_new_head(new_head(), ctx->cell->branch, ctx->head->closure);
        printf("-> %d\x1b[0m\n", ctx->head->closure->symbols->id);
        ctx->cell = ctx->cell->branch;
        /*
        if(ctx->cell && is_non_head_class(ctx->cell->value)){
            return;
        }
        */
        ctx->handle_state = CRW_IN_HEAD;
    }
    print_head(ctx->head);

    ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
    bool in_key = crw_process_keys(ctx);

    /* if we see keys in the open they can be skipped */
    if(!in_key){
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
