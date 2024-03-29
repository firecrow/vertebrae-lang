int number_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(is_allnum(c)){
    pattern->state = GKA_PARSE_IN_MATCH;
    string_append_char(pattern->token, c);
    return complete_previous(pattern, ctx);
  }else if(pattern->token->length){
    int value = atoi(pattern->token->content);
    finalize(ctx, new_int_value_obj(value));
    pattern->token = new_string();
  }
  pattern->state = GKA_PARSE_NOT_STARTED;
  return 0;
}
