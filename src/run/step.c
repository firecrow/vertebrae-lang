static void passthrough(struct head *head, struct head *previous){
    if(head && head->operator){
        head->operator->handle(head->operator, head, previous->value);
    }
}

struct crw_state *new_state_context(struct cell* root, struct closure *closure, struct stack_item *stack){
   struct crw_state *state = malloc(sizeof(struct state)); 

   if(state == NULL){
      return NULL;
   }

   memset(state, 0, sizeof(struct crw_state));

   state->closure = closure;
   state->cell = root;
   state->stack = stack;
   state->status = CRW_STATUS_CONTINUE;

   return state;
}

bool crw_next_step(struct step_state *state){
    if(ctx->cell->branch){
        /* creating the head will effectively process the cell */
        head = new_head(cell->branch, head);
        stack = push_stack(stack, cell, head);
        cell = cell->branch;

        spacing += 4;
    }else if(ctx->head){
        if(ctx->head->operator){
            struct value_obj *value = swap_for_symbol(ctx->closure, ctx->cell->value);
            if(value && value->type == SL_TYPE_CELL){
                value = value->slot.cell->value;
            }
            branch_type = ctx->head->operator->handle(head->operator, ctx->head, value);
            /* if the handle has communicated that it no longer wants to 
             * run the rest of the cells, setting cell->next to NULL here
             * will cause the if/else branch following to pull from the
             * previous stack entry
             */
            if(branch_type == SL_BREAK){
                cell->next = NULL;
            }
        }
    }

    ctx->cell = cell->next;

    if(ctx->cell == NULL){
        struct head *previous = ctx->head;
        ctx->stack = stack->previous;
        while(ctx->cell == NULL && ctx->stack){
            ctx->cell = ctx->stack->cell->next;
            ctx->head = ctx->stack->head;
            ctx->stack = ctx->stack->previous;
            passthrough(ctx->head, previous_head);
            spacing -= 4;
        }
    }
    return ctx->cell != NULL ? CRW_STATUS_CONTINUE : CRW_STATUS_DONE;
}
