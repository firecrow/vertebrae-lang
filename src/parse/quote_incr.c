void close_incr(struct cell_match_pattern *pattern, char c){
    if(c == '('){
      if(ctx->stack){
          ctx->current = ctx->stack->cell;
          ctx->stack = ctx->stack->previous;
      }else{
          ctx->current = NULL;
      }
      pattern->state = GKA_PARSE_DONE;
    }
}
