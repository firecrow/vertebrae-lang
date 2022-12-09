int close_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == ')' || c == ';' || c == ',' || c == '.'){
    pattern->state = GKA_PARSE_DONE;
    complete_previous(pattern, ctx);
    ctx->next_is_outof++;
    if(c == '.'){
        ctx->next_is_outof++;
    }

    if(debug){
        printf("poping to cell ");
        print_cell(ctx->cell);
        printf(", count: %d\n", parse_stack_count);
        
    }

    return 1;
  }
  return 0;
}
