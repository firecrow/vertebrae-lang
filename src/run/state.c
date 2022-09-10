#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

static void passthrough(struct crw_state *ctx, struct head *previous){
    ctx->value = previous->value;
    struct head *head = ctx->head;
    ctx->previous = previous;

    if(head->operator){
        head->operator->handle(head->operator, ctx);
    }else{
        ctx->value = head->value;
    }
    head->value = previous->value;
}

static struct stack_item *push_stack(struct crw_state *ctx, struct cell *cell){
    ctx->nesting++;
    struct stack_item *item = new_stack_item(ctx->stack, cell, ctx->head);
    return item;
}

static void pop_stack(struct crw_state *ctx){
    struct head *previous = ctx->head;
    ctx->head = ctx->stack->head;
    if(ctx->stack->cell){
        ctx->cell = ctx->stack->cell->next;
    }else{
        ctx->cell = NULL;
    }
    ctx->stack = ctx->stack->previous;
    passthrough(ctx, previous);
    ctx->nesting--;
}

static void next_step(struct crw_state *ctx);

static set_custom_cell_as_head(struct crw_state *ctx, struct cell *cell){
    ctx->stack = push_stack(ctx, ctx->cell);
    ctx->cell = cell;
    ctx->head = setup_new_head(new_head(), ctx->cell, ctx->head->closure);
    ctx->head->closure = ctx->head->closure;
}

static bool set_cell_func(struct crw_state *ctx){
    if(ctx->head->source && ctx->head->source->type == SL_TYPE_CELL){
        set_custom_cell_as_head(ctx, ctx->head->source->slot.cell);
        ctx->head->source = NULL;
        return 1;
    }
    return 0;
}

static void default_handle(struct operator_ifc *_op, struct crw_state *ctx){
    if(ctx->cell){
        ctx->cell = ctx->cell->next;
        if(ctx->cell == NULL){
            ctx->value = NULL;
            set_cell_func(ctx);
        }else{
            ctx->value = ctx->cell->value;
        }
    }
}

struct crw_state *crw_new_state_context(struct cell* root, struct closure *closure, struct stack_item *stack){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 

   if(state == NULL){
      return NULL;
   }


   memset(state, 0, sizeof(struct crw_state));

   state->builtins.true = new_result_value_obj(TRUE);
   state->builtins.false = new_result_value_obj(FALSE);
   state->builtins.nil = new_result_value_obj(NIL);
   state->builtins.error = new_result_value_obj(ERROR);

   state->head = setup_new_head(new_head(), root, closure);
   state->cell = root->next;
   state->stack = stack;
   state->status = CRW_CONTINUE;
   state->next = next_step;
   state->default_handle = default_handle;

   return state;
}

static void next_step(struct crw_state *ctx){
    if(!ctx->cell){
        fprintf(stderr, "Error next_step called on empty cell\n");
        exit(1);
    }

    /* if we see keys in the open they can be skipped */
    bool in_key = crw_process_keys(ctx);

    if(!in_key){
        if(ctx->cell->branch){
            ctx->stack = push_stack(ctx, ctx->cell);
            ctx->head = setup_new_head(new_head(), ctx->cell->branch, ctx->head->closure);
            ctx->cell = ctx->cell->branch;
        }

        ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
        tree_update(ctx->head->closure->symbols, str("value"), ctx->value);

        if(ctx->cell != ctx->head->cell){

            struct value_obj *head_value = swap_for_symbol(ctx->head->closure, ctx->head->cell->value);
            if(head_value->type == SL_TYPE_CELL){

                ctx->value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
                tree_update(ctx->head->closure->symbols, str("value"), ctx->value);

                set_custom_cell_as_head(ctx, head_value->slot.cell);

            }else if(ctx->head->operator){
                ctx->head->operator->handle(ctx->head->operator, ctx);
            }
        }else{
            ctx->default_handle(ctx->head->operator, ctx);
        }
    }else{
        ctx->default_handle(ctx->head->operator, ctx);
    }

    if(ctx->cell == NULL){
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
