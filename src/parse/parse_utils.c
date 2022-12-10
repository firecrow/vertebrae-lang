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

/**
 * create a cell that contains a value _containing_ a  cell so that it can be
 * used as a function pointer 
 */
void setup_quote_cell(struct parse_ctx *ctx, struct cell *new){
    struct cell *stack_cell = new_cell(NULL);
    if(debug){
        printf("setup func cells: stack");
        print_cell(stack_cell);
        printf("\n");
    }
    if(ctx->next_func_into == 1){
        struct cell *current = ctx->cell->prev;

        struct cell *func_name = ctx->cell;
        struct cell *func_cell = new;
        func_name->value->accent = GKA_PARSE_DEF;

        struct cell *nest =  new_cell(NULL);
        nest->branch = func_cell;
        struct cell *container =  new_cell(new_cell_value_obj(nest));
        ctx->stack = push_parse_stack(ctx->stack, container, NULL);
        ctx->stack = push_parse_stack(ctx->stack, nest, NULL);


        current->next = func_name;
        func_name->prev = current;
        container->prev = func_name;
        func_name->next = container;

        ctx->next_is_into--;

    }else{
        struct cell *blank = new_cell(NULL);
        stack_cell->branch = blank;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        blank->prev = ctx->cell;
    }

    ctx->accent = GKA_PARSE_NO_ACCENT;
}

static void append_branch_cell(struct parse_ctx *ctx, struct cell *stack_cell, struct cell *new, struct cell *previous){
    previous->next = new;
    stack_cell->branch = previous;
    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    ctx->cell = stack_cell;
}

static void setup_branch(struct parse_ctx *ctx, struct cell *new){
    struct cell *stack_cell = new_cell(NULL);
    struct cell *next = ctx->next;
    /*
    if(ctx->next_is_into > 1){
        next = new_cell(NULL);
    }
    */

    if(debug){
        printf("setup branch: cell/new/next/stack");
        printf("\n");
        print_cell(ctx->cell);
        printf("\n");
        print_cell(new);
        printf("\n");
        print_cell(next);
        printf("\n");
        print_cell(stack_cell);
        printf("\n");
    }
    struct cell *base = ctx->cell;

    ctx->cell->next = stack_cell;
    stack_cell->branch = next;
    ctx->cell = next;
    ctx->next = new;


    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    printf("finalize\n");

    if(value){
        if(ctx->accent == GKA_PARSE_QUOTE && !ctx->next_is_branch){
            value->accent = GKA_PARSE_QUOTE;
        }else{
            value->accent = ctx->accent;
        }
    }
    
    struct cell *new = new_cell(value);
    if(ctx->next_is_outof){
        printf("outof\n");
        while(ctx->next_is_outof){
            if(ctx->stack){
                ctx->cell->next = ctx->next;
                ctx->cell = ctx->stack->cell;
                ctx->stack = ctx->stack->previous;
                ctx->next = new;
                parse_stack_count--;
            }else{
                fprintf(stderr, "parse below stack error\n");
                exit(1);
            }
            ctx->next_is_outof--;
        }
    }else if(ctx->next_is_into || ctx->next_func_into){
        printf("into\n");
        while(ctx->next_func_into > 0){
            indent++;
            setup_quote_cell(ctx, new); 
            ctx->next_func_into--;
        }
        while(ctx->next_is_into > 0){
            indent++;
            setup_branch(ctx, new);
            ctx->next_is_into--;
        }
    }else{
        if(!ctx->root){
            if(debug){
                printf("setup root: next\n");
                print_cell(new);
                printf("\n");
            }
            struct cell *root = new_cell(NULL); 
            ctx->cell = ctx->root = root; 
            ctx->cell->branch = new_cell(NULL);
            ctx->cell = ctx->cell->branch;
            ctx->next = new;
        }else{
            if(debug){
                printf("normal: next/new ");
                print_cell(ctx->next);
                print_cell(new);
                printf("\n");
            }
            ctx->cell->next = ctx->next;
            ctx->cell = ctx->next;
            ctx->next = new;
        }

        ctx->accent = GKA_PARSE_NO_ACCENT;
    }
    if(debug){
        printf("parse stack is %d\n",  parse_stack_count);
        print_space(indent);
        printf("P\n");
    }
}

void finalize_parse(struct parse_ctx *ctx){
    ctx->current->incr(ctx->current, ctx, '\0');
    finalize(ctx, (struct value_obj *)NULL);
}
