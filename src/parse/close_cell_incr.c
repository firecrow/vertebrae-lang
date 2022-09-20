void close_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == ')'){
    if(ctx->stack){
        ctx->cell = ctx->stack->cell;
        ctx->stack = ctx->stack->previous;
    }else{
        ctx->cell = NULL;
    }
    pattern->state = GKA_PARSE_DONE;
  }
}
