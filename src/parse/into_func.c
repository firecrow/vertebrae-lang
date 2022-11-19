int into_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(pattern->state == GKA_PARSE_NOT_STARTED && c == '>'){
      pattern->state = GKA_PARSE_IN_MATCH;
      return complete_previous(pattern, ctx);
  }
  if(pattern->state == GKA_PARSE_IN_MATCH){
      if(c == '>'){
          ctx->next_func_into++;
          pattern->state = GKA_PARSE_DONE;
      }else{
          pattern->state = GKA_PARSE_NOT_STARTED;
      }
      return 1;
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
