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

void resolve_next(struct parse_ctx *ctx, struct cell *next){
    if(next && next->value && (
                next->value->accent == GKA_PARSE_HEAD ||
                next->value->accent == GKA_PARSE_DEF ||
                next->value->accent == GKA_PARSE_SET
            )){
        ctx->cell->branch = next;
    }else{
        ctx->cell->next = next;
    }
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
void setup_func_cell(struct parse_ctx *ctx, struct cell *new){
    struct value_obj *stack_value = new_value();
    stack_value->accent = GKA_PARSE_HEAD;
    struct cell *stack_cell = new_cell(stack_value);
    if(debug){
        printf("setup func cells: stack/next/new\n");
        print_cell(stack_cell);
        printf("\n");
        print_cell(ctx->next);
        printf("\n");
        print_cell(new);
        printf("\n");
    }
    if(ctx->next_func_into == 1){
        struct cell *func_name = ctx->next;
        func_name->value->accent = GKA_PARSE_DEF;
        struct cell *func_cell = new_cell(new_cell_value_obj(stack_cell));
        new->value->accent = GKA_PARSE_HEAD;
        ctx->stack = push_parse_stack(ctx->stack, func_cell, NULL);
        

        func_name->next = func_cell;

        resolve_next(ctx, func_name);
        ctx->cell = stack_cell;
        
        ctx->next = new;

    }else{
        struct cell *blank = new_cell(NULL);
        stack_cell->branch = blank;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        resolve_next(ctx, stack_cell);
        ctx->cell = blank;
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
    struct cell *next = ctx->next;
    struct value_obj *stack_value = new_value();
    /*
    if(next->value->accent || ctx->cell->value->accent){
        stack_value->accent = GKA_PARSE_HEAD;
    }
    */
    struct cell *stack_cell = new_cell(stack_value);

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

    if(next && next->value && (
                next->value->accent == GKA_PARSE_HEAD ||
                next->value->accent == GKA_PARSE_DEF ||
                next->value->accent == GKA_PARSE_SET
            )){
        resolve_next(ctx, next);
        ctx->stack = push_parse_stack(ctx->stack, next, NULL);
    }else{
        resolve_next(ctx, stack_cell);
        stack_cell->branch = next;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    }
    ctx->cell = next;
    ctx->next = new;
}

static void finalize(struct parse_ctx *ctx, struct value_obj *value){
    if(debug){
        printf("\nfinalize outof %d into %d func %d\n", ctx->next_is_outof, ctx->next_is_into, ctx->next_func_into);
        print_value(value);
        printf("\n");
    }

    if(value){
        if(ctx->accent == GKA_PARSE_QUOTE && !ctx->next_is_branch){
            value->accent = GKA_PARSE_QUOTE;
        }else{
            value->accent = ctx->accent;
        }
    }
    
    struct cell *new = new_cell(value);
    if(ctx->next_is_outof){
        if(debug){
            printf("outof\n");
        }
        int resolved = 0;
        while(ctx->next_is_outof){
            if(ctx->stack){
                if(!resolved){
                    resolve_next(ctx, ctx->next);
                    resolved = 1;
                }
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
        if(debug){
            printf("into\n");
        }
        while(ctx->next_func_into > 0){
            if(debug){
                printf("func\n");
            }
            indent++;
            setup_func_cell(ctx, new); 
            ctx->next_func_into--;
        }
        while(ctx->next_is_into > 0){
            if(debug){
                printf("branch\n");
            }
            indent++;
            setup_branch(ctx, new);
            ctx->next_is_into--;
        }
    }else{
        if(debug){
            printf("normal...\n");
        }
        if(!ctx->root){
            ctx->stack = NULL;
            if(debug){
                printf("setup root: next\n");
                print_cell(new);
                printf("\n");
            }
            struct cell *root = new_cell(new_value()); 
            ctx->cell = ctx->root = root; 
            struct cell *stack_cell = new_cell(new_value()); 
            ctx->cell->branch = stack_cell;
            ctx->cell = stack_cell;
            if(!new->value->accent){
                new->value->accent = GKA_PARSE_HEAD;
            }
            ctx->next = new;
        }else{
            if(debug){
                printf("normal: next/new ");
                print_cell(ctx->next);
                print_cell(new);
                printf("\n");
            }
            resolve_next(ctx, ctx->next);
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
    printf("finalize...\n");
    ctx->current->incr(ctx->current, ctx, '\0');
    finalize(ctx, (struct value_obj *)NULL);
}
