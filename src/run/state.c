#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../core/core.h"
#include "../operators/operator.h"
#include "run.h"

static void next_step(struct crw_state *ctx);

static void passthrough(struct head *head, struct head *previous){
    if(head && head->operator){
        head->operator->handle(head->operator, head, previous->value);
    }
}

struct crw_state *crw_new_state_context(struct cell* root, struct closure *closure, struct stack_item *stack){
   struct crw_state *state = malloc(sizeof(struct crw_state)); 

   if(state == NULL){
      return NULL;
   }

   memset(state, 0, sizeof(struct crw_state));

   state->closure = closure;
   state->cell = root;
   state->stack = stack;
   state->status = CRW_CONTINUE;
   state->next = next_step;

   return state;
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
}

static void next_step(struct crw_state *ctx){
    printf("1\n");
    print_cell(ctx->cell);
    if(ctx->cell->branch){
        /* creating the head will effectively process the cell */
        ctx->head = new_head(ctx->cell->branch, ctx->head);
        ctx->stack = push_stack(ctx->stack, ctx->cell, ctx->head);
    }else if(ctx->head){
        if(ctx->head->operator){
            struct value_obj *value = swap_for_symbol(ctx->closure, ctx->cell->value);
            if(value && value->type == SL_TYPE_CELL){
                value = value->slot.cell->value;
            }
            enum SL_BRANCH_TYPE branch_type = ctx->head->operator->handle(ctx->head->operator, ctx->head, value);
            /* if the handle has communicated that it no longer wants to 
             * run the rest of the cells, setting cell->next to NULL here
             * will cause the if/else branch following to pull from the
             * previous stack entry
             */
            if(branch_type == SL_BREAK){
                pop_stack(ctx);
                return CRW_CONTINUE;
            }
        }
    }
    printf("2\n");

    if(ctx->cell->branch){
        ctx->cell = ctx->cell->branch;
    }else{
        ctx->cell = ctx->cell->next;
    }

    printf("3\n");
    if(ctx->cell == NULL){
        while(ctx->cell == NULL && ctx->stack){
            pop_stack(ctx);
        }
    }
    printf("4 %d\n", ctx->cell == NULL);
    if(!ctx->cell){
        ctx->status = CRW_DONE;
        return;
    }
    ctx->status =  CRW_CONTINUE;
}
