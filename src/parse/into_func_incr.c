int into_func_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  printf(">>>>>>>>>>>>>>>> 0\n");
  if(pattern->state == GKA_PARSE_NOT_STARTED && c == '>'){
      printf(">>>>>>>>>>>>>>>> 1\n");
      pattern->state = GKA_PARSE_IN_MATCH;
      return complete_previous(pattern, ctx);
  }
  if(pattern->state == GKA_PARSE_IN_MATCH){
      if(c == '>'){
          printf(">>>>>>>>>>>>>>>> 2\n");
          ctx->next_func_into++;
          pattern->state = GKA_PARSE_DONE;
          return complete_previous(pattern, ctx);
      }else{
          pattern->state = GKA_PARSE_NOT_STARTED;
          return 1;
      }
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
