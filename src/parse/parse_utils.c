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

static void append_branch_cell(struct parse_ctx *ctx, struct cell *stack_cell, struct cell *new, struct cell *previous){
    printf("\x1b[31mappend_branch_cell\x1b[0m\n");
    printf("\x1b[31mprevious: ");
    print_cell(ctx->previous);
    printf("\n");
    printf("\x1b[31mstack_cell: ");
    print_cell(stack_cell);
    printf("\n");
    printf("\x1b[31mnew: ");
    print_cell(new);
    printf("\x1b[0m\n");
    /* set the previous cell as a stack cell */
    stack_cell->branch = previous;
    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    ctx->cell = previous;

    /* now append the new cell */
    ctx->cell->next = new;
    ctx->cell = new;

    set_previous(ctx);
}

/**
 * create a cell that contains a value _containing_ a  cell so that it can be
 * used as a function pointer 
 */
void setup_quote_cell(struct parse_ctx *ctx, struct cell *new){
    printf("\x1b[31msetup cell\x1b[0m\n");

    struct cell *stack_cell = new_cell(NULL);
    if(ctx->next_func_into == 1){

        struct cell *container = new_cell_value_obj(ctx->previous);
        struct cell *previous = new_cell(container);

        if(debug){
            printf("in qetup_quote_cell assign\n");
            printf("quote_cell\n");
            print_cell(previous);
            printf("\n");
        }

        ctx->grand_previous->next = stack_cell;
        append_branch_cell(ctx, stack_cell, new, previous);

    }else{
        struct cell *blank = new_cell(NULL);
        if(debug){
            printf("in qetup_quote_cell climb\n");
        }
        stack_cell->branch = blank;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        ctx->cell = blank; 
    }

    if(debug){
        printf("new\n");
        print_cell(new);
        printf("\n");
    }

    ctx->accent = GKA_PARSE_NO_ACCENT;
}
static void setup_branch(struct parse_ctx *ctx, struct cell *new){
    printf("\x1b[35msetting up branch\n\x1b[0m");
    struct cell *stack_cell = new_cell(NULL);
    if(ctx->next_is_into == 1){
        if(ctx->previous){
            ctx->grand_previous->next = stack_cell;
            append_branch_cell(ctx, stack_cell, new, ctx->previous);
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
    if(ctx->next_is_into || ctx->next_func_into){
        while(ctx->next_func_into){
            setup_quote_cell(ctx, new); 
            ctx->next_func_into--;
        }
        while(ctx->next_is_into){
            setup_branch(ctx, new);
            ctx->next_is_into--;
        }
    }else{
        if(!ctx->root){
            struct cell *previous = new_cell(NULL);
            struct cell *stack_cell = new_cell(NULL);
            struct cell *new = new_cell(NULL);
            ctx->root = ctx->previous = ctx->cell = previous;
            ctx->cell->next = stack_cell;
            ctx->cell = stack_cell;
            append_branch_cell(ctx, stack_cell, new, ctx->previous);
            set_previous(ctx);
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
