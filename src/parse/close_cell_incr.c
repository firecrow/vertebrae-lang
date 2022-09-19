void close_cell_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == ')'){
    pattern->state = GKA_PARSE_DONE;
  }
}
