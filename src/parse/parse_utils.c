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

void set_previous(struct parse_ctx *ctx){
  if(ctx->previous){
    ctx->grand_previous = ctx->previous;
  }
  ctx->previous = ctx->cell;
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

/**
 * create a cell that contains a value _containing_ a  cell so that it can be
 * used as a function pointer 
 */
void setup_quote_cell(struct parse_ctx *ctx, struct value_obj *value, struct cell *new, struct cell *previous, struct cell *grand_previous){
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
    set_previous(ctx);

    if(ctx->next_is_into == 1){
      /* make the branch cell the previous cell
      stack_cell->branch = previous;
      ctx->cell = previous;
      /* now append the new cell */
      ctx->cell->next = new;
      ctx->previous = ctx->cell;
      ctx->cell = new;
      set_previous(ctx);
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
}

static void append_branch_cell(struct parse_ctx *ctx, struct cell *stack_cell, struct cell *new){
    /* set the previous cell as a stack cell */
    stack_cell->branch = ctx->previous;
    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    ctx->cell = ctx->previous;

    /* now append the new cell */
    ctx->cell->next = new;
    ctx->previous = ctx->cell;
    ctx->cell = new;

    set_previous(ctx);
}


static void setup_branch(struct parse_ctx *ctx, struct cell *new){
    printf("\x1b[35msetting up branch\n\x1b[0m");
    struct cell *stack_cell = new_cell(NULL);
    if(ctx->next_is_into == 1){
        if(!ctx->grand_previous){
          ctx->root = ctx->cell = ctx->grand_previous = stack_cell;
        }
        if(ctx->previous){
            ctx->grand_previous->next = stack_cell;
            append_branch_cell(ctx, stack_cell, new);
        }
        ctx->cell = new;
    }else{
        struct cell *blank = new_cell(NULL);
        stack_cell->branch = blank;
        if(!ctx->cell){
            ctx->cell = new_cell(NULL);
        }
        ctx->cell->next = stack_cell;
        ctx->previous = ctx->cell;
        ctx->cell = blank;

        if(!ctx->root){
            ctx->root = ctx->cell;
        }

    }
}


static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    if(debug){
        printf("\x1b[36mfinalize: %d %d", ctx->next_is_into, ctx->next_func_into);
        print_value(value);
        printf("\x1b[0m\n");
    }
    fflush(stdout);

    if(ctx->accent == GKA_PARSE_QUOTE && !ctx->next_is_branch){
        value->accent = GKA_PARSE_QUOTE;
    }

    struct cell *new = new_cell(value);
    struct cell *stack_cell = new_cell(value);
    if(ctx->next_is_into || ctx->next_func_into){
        while(ctx->next_func_into){
            setup_quote_cell(ctx, value, new, ctx->previous, ctx->cell); 
            ctx->next_func_into--;
        }
        while(ctx->next_is_into){
            setup_branch(ctx, new);
            ctx->next_is_into--;
        }
    }else{
        if(!ctx->root){
            struct cell *blank = new_cell(NULL);
            struct cell *stack_cell = new_cell(NULL);
            struct cell *new = new_cell(NULL);
            ctx->root = ctx->cell = ctx->previous = blank;
            append_branch_cell(ctx, stack_cell, new);
        }

        if(debug){
            printf("\x1b[35madding to current cell: ");
            print_cell(ctx->cell);
            printf("\x1b[0m\n");
        }

        ctx->cell->next = new;
        ctx->cell = new;
        set_previous(ctx);
        ctx->accent = GKA_PARSE_NO_ACCENT;
    }
}

void finalize_parse(struct parse_ctx *ctx){
    ctx->current->incr(ctx->current, ctx, '\0');
}
