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
    if(pattern->state != GKA_PARSE_NOT_STARTED){
        /* close the current pattern if exists */
        if(ctx->current && ctx->current != pattern && ctx->current->state == GKA_PARSE_IN_MATCH){
            ctx->current->incr(ctx->current, ctx, '\0');
        }
        if(pattern->state == GKA_PARSE_DONE){
            pattern->state = GKA_PARSE_NOT_STARTED;
        }else{
            ctx->current = pattern;
        }
        return 1;
    }
    return 0;
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    /*
    ctx->cell->value = value;
    ctx->cell->next = new_cell(NULL);
    ctx->cell = ctx->cell->next;
    */
    struct cell *new = new_cell(value);
    printf("new: ");
    print_cell(new);
    printf("\n");
    if(ctx->cell){
        if(ctx->next_is_branch){
            ctx->cell->branch = new;
        }else {
            ctx->cell->next = new;
        }
        ctx->cell = new;
    }else{
        ctx->cell = new;
    }
}
