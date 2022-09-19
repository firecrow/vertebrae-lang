void number_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(is_allnum(c)){
    pattern->state = GKA_PARSE_IN_MATCH;
    string_append_char(pattern->token, c);
    return;
  }else if(pattern->token->length){
    finalize(ctx, new_int_value_obj(atoi(pattern->token)));
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
}
