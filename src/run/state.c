#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

static void passthrough(struct head *head, struct head *previous){
    if(head->operator){
        head->operator->handle(head->operator, head, previous->value);
    }else{
        head->value = previous->value;
    }
}

static struct stack_item *push_stack(struct crw_state *ctx){
    ctx->nesting++;
    struct stack_item *item = new_stack_item(ctx->stack, ctx->cell, ctx->head);
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
    passthrough(ctx->head, previous);
    ctx->nesting--;
}

static void next_step(struct crw_state *ctx);

static void default_handle(struct operator_ifc *_op, struct crw_state *ctx){
    ctx->cell = ctx->cell->next;
}

struct crw_state *crw_new_state_context(struct cell* root, struct closure *closure, struct stack_item *stack){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 

   if(state == NULL){
      return NULL;
   }

   memset(state, 0, sizeof(struct crw_state));

   state->head = setup_new_head(new_head(), root, closure);
   state->cell = root->next;
   state->stack = stack;
   state->status = CRW_CONTINUE;
   state->next = next_step;
   state->default_handle = default_handle;

   return state;
}

static bool set_cell_func(struct crw_state *ctx){
    if(ctx->head->source && ctx->head->source->type == SL_TYPE_CELL){
        ctx->stack = push_stack(ctx);
        ctx->cell = ctx->head->source->slot.cell;
        ctx->head = setup_new_head(new_head(), ctx->cell, ctx->head->closure);
        ctx->head->closure = ctx->head->closure;
        ctx->head->source = NULL;
        return 1;
    }
    return 0;
}

static void next_step(struct crw_state *ctx){
    if(!ctx->cell){
        fprintf(stderr, "Error next_step called on empty cell\n");
        exit(1);
    }

    /*
    printf("               >cell on start of next: ");
    print_cell(ctx->cell);
    printf("\n");
    */

    struct value_obj *value = swap_for_symbol(ctx->head->closure, ctx->cell->value);
    tree_update(ctx->head->closure->symbols, str("value"), value);

    /* if we see keys in the open they can be skipped */
    bool in_key = crw_process_keys(ctx);

    if(in_key){
        ctx->cell = ctx->cell->next;
        if(ctx->cell == NULL && set_cell_func(ctx)){
            ctx->cell = ctx->cell->next;
        }
        if(!ctx->cell){
            ctx->status = CRW_DONE;
        }
        return;
    }
    if(ctx->cell->branch){
        ctx->stack = push_stack(ctx);
        ctx->head = setup_new_head(new_head(), ctx->cell->branch, ctx->head->closure);
        ctx->cell = ctx->cell->branch;
    }else{
        bool was_cell_func = set_cell_func(ctx);

        if(!was_cell_func && ctx->head->operator){
            ctx->head->operator->handle(ctx->head->operator, ctx->head, value);
        }
    }

    ctx->default_handle(ctx->head->operator, ctx);

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
