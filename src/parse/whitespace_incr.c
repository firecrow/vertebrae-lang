int whitespace_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == '\n' || c == ' ' || c == '\t'){
    pattern->state = GKA_PARSE_DONE;
    return complete_previous(pattern, ctx);
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
