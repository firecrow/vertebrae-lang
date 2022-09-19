void symbol_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
  if(c == '\0'){
    /* add cell here */;
  }
  string_append_char(pattern->token, c);
}
