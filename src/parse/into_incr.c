int into_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(pattern->state == GKA_PARSE_NOT_STARTED && c == '<'){
      pattern->state = GKA_PARSE_DONE;
      complete_previous(pattern, ctx);
      ctx->next_is_into++;
      return 1;
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
