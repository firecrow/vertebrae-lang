int open_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  struct cell *slot;
  struct cell *stack_cell;
  if(c == '('){
    ctx->next_is_branch = 1;
    struct cell *new = new_cell(NULL);
    if(ctx->accent == GKA_PARSE_QUOTE){
        ctx->cell->value = new_cell_value_obj(new);
        ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        ctx->cell = new;
        ctx->accent = GKA_PARSE_NO_ACCENT;
    }else{
        if(ctx->cell){
            ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
            ctx->cell = new_cell(NULL);
        }else{
            if(!ctx->root){
                ctx->root = new;
            }
            ctx->cell = new;
        }

    }
    pattern->state = GKA_PARSE_DONE;
    return complete_previous(pattern, ctx);
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
