int close_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == ')' || c == ';' || c == ','){
    pattern->state = GKA_PARSE_DONE;
    complete_previous(pattern, ctx);
    if(ctx->stack){
        ctx->cell = ctx->stack->cell;
        ctx->stack = ctx->stack->previous;
    }else{
        ctx->cell = NULL;
    }

    if(debug){
        printf("poping to cell ");
        print_cell(ctx->cell);
        printf("\n");
    }

    return 1;
  }
  return 0;
}
