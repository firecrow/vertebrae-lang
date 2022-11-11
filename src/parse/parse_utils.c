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
        if(ctx->current && (ctx->current != pattern) && (ctx->current->state == GKA_PARSE_IN_MATCH)){
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

    if(debug){
        printf("\x1b[36mfinalize: %d ", ctx->next_is_branch);
        print_value(value);
        printf("\x1b[0m\n");
    }

    if(ctx->accent == GKA_PARSE_QUOTE){
        value->accent = GKA_PARSE_QUOTE;
    }

    struct cell *new = new_cell(value);
    struct cell *stack_cell = new_cell(value);
    if(ctx->next_is_branch){
        while(ctx->next_is_branch){
            if(ctx->accent == GKA_PARSE_QUOTE){

                struct cell *stack_cell = new_cell(NULL);
                struct cell *blank = new_cell(NULL);

                struct cell *container = new_cell_value_obj(stack_cell);
                struct cell *quoted_new = new_cell(container);

                if(debug){
                    printf("quote_cell\n");
                    print_cell(quoted_new);
                    printf("\n");
                }

                ctx->stack = push_parse_stack(ctx->stack, quoted_new, NULL);
                ctx->cell->next = quoted_new;

                if(ctx->next_is_branch == 1){
                    stack_cell->branch = new;
                    ctx->cell = new; 
                }else{
                    stack_cell->branch = blank;
                    ctx->cell = blank; 
                }

                if(debug){
                    printf("new\n");
                    print_cell(new);
                    printf("\n");
                }

                if(!ctx->root){
                    ctx->root = ctx->cell;
                }

                ctx->accent = GKA_PARSE_NO_ACCENT;

            }else{
                struct cell *stack_cell = new_cell(NULL);
                ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
                if(ctx->next_is_branch == 1){
                    stack_cell->branch = new;
                    if(ctx->cell){
                        ctx->cell->next = stack_cell;
                    }else{
                        ctx->cell = ctx->root = stack_cell;
                    }
                    ctx->cell = new;
                }else{
                    struct cell *blank = new_cell(NULL);
                    stack_cell->branch = blank;
                    if(!ctx->cell){
                        ctx->cell = new_cell(NULL);
                    }
                    ctx->cell->next = stack_cell;
                    ctx->cell = blank;

                    if(!ctx->root){
                        ctx->root = ctx->cell;
                    }

                }
            }

            ctx->next_is_branch--;
        }

        return;

    }else{
        /*
        printf("-----------> extending: ");
        print_cell(ctx->cell);
        printf("-----> ");
        print_cell(new);
        printf("\n");
        */
        ctx->cell->next = new;
    }
    ctx->cell = new;
    ctx->accent = GKA_PARSE_NO_ACCENT;
}
