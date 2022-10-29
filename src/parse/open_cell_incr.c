int open_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  struct cell *slot;
  struct cell *stack_cell;
  if(c == '('){
    ctx->next_is_branch++;
    pattern->state = GKA_PARSE_DONE;
    return complete_previous(pattern, ctx);
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
