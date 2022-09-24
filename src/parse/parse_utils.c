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

static int complete_previous(struct match_pattern *pattern, struct parse_ctx *ctx){
    printf("1\n");
    if(pattern->state != GKA_PARSE_NOT_STARTED){
        printf("2\n");
        /* close the current pattern if exists */
        if(ctx->current && ctx->current != pattern && ctx->current->state == GKA_PARSE_IN_MATCH){
            ctx->current->incr(ctx->current, ctx, '\0');
        }
        printf("3\n");
        if(pattern->state == GKA_PARSE_DONE){
            pattern->state = GKA_PARSE_NOT_STARTED;
        }else{
            printf("assigning current\n");
            ctx->current = pattern;
        }
        printf("4\n");
        return 1;
    }
    printf("5\n");
    return 0;
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    printf("in finalize");
    print_value(value);
    printf("\n");
    ctx->cell->value = value;
    ctx->cell->next = new_cell(NULL);
    ctx->cell = ctx->cell->next;
}
