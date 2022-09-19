void open_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  struct cell *slot;
  struct cell *stack_cell;
  if(c == '('){
    struct cell *new = new_cell();
    if(ctx->accent == GKA_PARSE_QUOTE){
        ctx->cell->value = new_cell_value_obj(new);
        ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        ctx->cell = new;
    }else{
        stack_cell = new_cell();
        stack_cell->branch = new;

        slot = ctx->cell;

        ctx->cell = new;
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
    return;
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
}
