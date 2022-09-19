void whitespace_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == '\0' || c == '\n' || c == ' ' || c == '\t'){
    pattern->state = GKA_PARSE_DONE;
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
}
