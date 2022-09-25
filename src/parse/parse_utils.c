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
    printf("finalize: %d ", ctx->next_is_branch);
    print_value(value);
    printf("\n");
    */

    struct cell *new = new_cell(value);
    struct cell *stack_cell = new_cell(value);
    if(ctx->next_is_branch){
        if(ctx->accent == GKA_PARSE_QUOTE){

            struct cell *quoted_new = new_cell(value);
            quoted_new->value = new_cell_value_obj(new);

            printf("quote_cell\n");
            print_cell(quoted_new);
            printf("\n");

            ctx->stack = push_parse_stack(ctx->stack, quoted_new, NULL);
            ctx->cell->next = new;
            ctx->cell = ctx->cell->next; 

            printf("new\n");
            print_cell(new);
            printf("\n");

            ctx->accent = GKA_PARSE_NO_ACCENT;
            ctx->next_is_branch = 0;

            return;
        }

        if(ctx->cell){
            ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        }else{
            struct cell *root_cell = new_cell(NULL);
            if(!ctx->root){
                ctx->root = root_cell;
            }
            ctx->cell = root_cell;
        }

        printf("making branch of off: ");
        print_cell(ctx->cell);
        printf(" -> ");
        print_cell(new);
        printf("\n");

        ctx->cell->branch = new;
        ctx->cell = ctx->cell->branch;

        ctx->next_is_branch = 0;

        return;

    }else{
        ctx->cell->next = new;
    }
    ctx->cell = new;
}
