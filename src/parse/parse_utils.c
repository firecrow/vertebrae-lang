bool is_allnum(char c){
  if(c >= '0' && c <= '9'){
    return 1;
  }
  return 0;
}

bool is_alpha(char c){
  if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
    return 1;
  }
  return 0;
}

bool is_alphanum(char c){
  return is_alpha(c) || is_allnum(c);
}

bool is_whitespace(char c){
    return c == ' ' || c == '\t' || c == '\n';
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    ctx->cell->value = value;
    printf("\x1b[35m");
    print_cell(ctx->cell);
    printf("\x1b[0m\n");
    ctx->cell->next = new_cell(NULL);
    ctx->cell = ctx->cell->next;
}
