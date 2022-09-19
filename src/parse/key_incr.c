void key_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(pattern->state == GKA_PARSE_NOT_STARTED && c == '.'){
    pattern->state = GKA_PARSE_IN_MATCH;
    return;
  }
  if(pattern->state == GKA_PARSE_IN_MATCH){
    if(is_alphanum(c) || c == '-'){
      string_append_char(pattern->token, c);
    }else{
      pattern->state = GKA_PARSE_NOT_STARTED;
    }
  }
}
