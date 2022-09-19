void open_incr(struct cell_match_pattern *pattern, char c){
  if(c == '('){
    new = new_cell();
    if(ctx->accent == GKA_PARSE_IN_QUOTE){
        ctx->current->value = new_cell_value_obj(new);
        ctx->stack = push_parse_stack(ctx->stack, ctx->current, NULL);
        ctx->current = new;
    }else{
        stack_cell = new_cell();
        stack_cell->branch = new;

        slot = ctx->current;

        ctx->current = new;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);

        if(!ctx->root){
            ctx->root = stack_cell;
        }else{
            if(slot){
                slot->next = stack_cell;
            }
        }
    }
    pattern->state = GKA_PARSE_DONE;
  }
}
