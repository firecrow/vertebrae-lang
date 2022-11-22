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

static void append_branch_cell(struct parse_ctx *ctx, struct cell *stack_cell, struct cell *new, struct cell *previous){
    if(debug){
        printf("\x1b[31mappend_branch_cell\x1b[0m\n");
        printf("\x1b[31mprevious: ");
        print_cell(previous);
        printf("\n");
        printf("\x1b[31mstack_cell: ");
        print_cell(stack_cell);
        printf("\n");
        printf("\x1b[31mnew: ");
        print_cell(new);
        printf("\x1b[0m\n");
    }

    /* set the previous cell as a stack cell */
    stack_cell->branch = previous;
    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    ctx->cell = previous;

    /* now append the new cell */
    ctx->cell->next = new;
    new->prev = ctx->cell;
    ctx->cell = new;
}

/**
 * create a cell that contains a value _containing_ a  cell so that it can be
 * used as a function pointer 
 */
void setup_quote_cell(struct parse_ctx *ctx, struct cell *new){
    if(debug){
        printf("\x1b[31msetup quote cell\x1b[0m\n");
    }

    struct cell *stack_cell = new_cell(NULL);
    if(ctx->next_func_into == 1){
        struct cell *current = ctx->cell->prev;

        struct cell *func_name = ctx->cell;
        struct cell *func_cell = new;
        func_name->value->accent = GKA_PARSE_DEF;

        struct cell *nest =  new_cell(NULL);
        nest->branch = func_cell;
        struct cell *container =  new_cell(new_cell_value_obj(nest));
        ctx->stack = push_parse_stack(ctx->stack, container, NULL);

        container->prev = current;
        current->next = container;
        current->next = func_name;
        func_name->prev = current;
        func_name->next = container;
        ctx->cell = func_cell; 

        if(debug){
            printf("\x1b[36mcurrent\n");
            print_cell(current);
            printf("\nstack\n");
            print_cell(stack_cell);
            printf("\ncontaiener\n");
            print_cell(container);
            print_value(container->value);
            printf("\ncctx->cell\n");
            print_cell(ctx->cell);
            printf("\ncctx->cell->prev\n");
            print_cell(ctx->cell->prev);

            printf("\nfunc_name\n");
            print_cell(func_name);
            printf("\nfunc_cell\n");
            print_cell(func_cell);
            printf("\x1b[0m\n");
        }

        ctx->next_is_into--;

    }else{
        struct cell *blank = new_cell(NULL);
        if(debug){
            printf("in qetup_quote_cell climb\n");
        }
        stack_cell->branch = blank;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        blank->prev = ctx->cell;
        ctx->cell = blank; 
    }

    if(debug){
        printf("new cell\n");
        print_cell(new);
        printf("\n");
    }

    ctx->accent = GKA_PARSE_NO_ACCENT;
}
static void setup_branch(struct parse_ctx *ctx, struct cell *new){
    struct cell *stack_cell = new_cell(NULL);
    if(ctx->next_is_into == 1){
        if(ctx->cell->prev){
            ctx->cell->prev->next = stack_cell;
            append_branch_cell(ctx, stack_cell, new, ctx->cell);
        }
        new->prev = ctx->cell;
        ctx->cell = new;
    }else{
        struct cell *blank = new_cell(NULL);
        stack_cell->branch = blank;
        if(!ctx->cell){
            struct cell *_new = new_cell(NULL);
            _new->prev = ctx->cell;
            ctx->cell = _new;
        }
        ctx->cell->next = stack_cell;
        stack_cell->prev = ctx->cell;
        blank->prev = ctx->cell;
        ctx->cell = blank;
    }
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){

    if(ctx->accent == GKA_PARSE_QUOTE && !ctx->next_is_branch){
        value->accent = GKA_PARSE_QUOTE;
    }else{
        value->accent = ctx->accent;
    }

    struct cell *new = new_cell(value);
    if(debug){
        printf("\x1b[36mfinalize: %d %d", ctx->next_is_into, ctx->next_func_into);
        print_cell(new);
        printf("\x1b[0m\n");
    }
    if(ctx->next_is_into || ctx->next_func_into){
        while(ctx->next_func_into > 0){
            if(debug){
                printf("\x1b[35mbefore: ");
                print_cell(ctx->cell);
                printf("\x1b[0m\n");
            }
            setup_quote_cell(ctx, new); 
            ctx->next_func_into--;
            if(debug){
                printf("\x1b[35mafter: ");
                print_cell(ctx->cell);
                printf("\x1b[0m\n");
            }
        }
        while(ctx->next_is_into > 0){
            setup_branch(ctx, new);
            ctx->next_is_into--;
        }
    }else{
        if(!ctx->root){
            struct cell *previous = new_cell(NULL);
            struct cell *stack_cell = new_cell(NULL);
            ctx->root = ctx->cell->prev = ctx->cell = previous;
            ctx->cell->next = stack_cell;
            stack_cell->prev = ctx->cell;
            ctx->cell = stack_cell;
            struct cell *next = new_cell(NULL);
            append_branch_cell(ctx, stack_cell, next, ctx->cell->prev);
        }

        if(debug){
            printf("\x1b[35madding to current cell: ");
            print_cell(ctx->cell);
            printf("\x1b[0m\n");
        }

        fflush(stdout);

        ctx->cell->next = new;
        new->prev = ctx->cell;
        ctx->cell = new;
        ctx->accent = GKA_PARSE_NO_ACCENT;
    }
}

void finalize_parse(struct parse_ctx *ctx){
    ctx->current->incr(ctx->current, ctx, '\0');
}
