void open_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  struct cell *slot;
  struct cell *stack_cell;
  if(c == '('){
    struct cell *new = new_cell(NULL);
    if(ctx->accent == GKA_PARSE_QUOTE){
        ctx->cell->value = new_cell_value_obj(new);
        ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        ctx->cell = new;
    }else{
        ctx->cell->branch = new;
        ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        ctx->cell = new;
    }
    pattern->state = GKA_PARSE_DONE;
    return;
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
}
