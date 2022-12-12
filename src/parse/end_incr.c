int end_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == '\0'){
    pattern->state = GKA_PARSE_DONE;
    complete_previous(pattern, ctx);
    finalize(ctx, new_value());
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
