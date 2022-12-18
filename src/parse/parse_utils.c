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
    if(next->is_head){
        if(ctx->cell->branch){
            fprintf(stderr, "error has branch already\n");
            /*
            exit(1);
            */
        }
        ctx->cell->branch = next;
    }else{
        if(ctx->cell->next){
            fprintf(stderr, "error has next cell already\n");
            /*
            exit(1);
            */
        }
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
    struct cell *stack_cell = new_cell(new_value());
    stack_cell->is_head = 1;

    struct cell *name_stack_cell = new_cell(new_value());

    if(debug){
        printf("setup func cells: stack/next/new\n");
        print_cell(stack_cell);
        printf("\n");
        print_cell(ctx->next);
        printf("\n");
        print_cell(new);
        printf("\n---------\n");
    }
    if(ctx->next_func_into == 1){

        resolve_next(ctx, name_stack_cell);
        ctx->stack = push_parse_stack(ctx->stack, name_stack_cell, NULL);
        ctx->cell = name_stack_cell;

        struct cell *func_name = ctx->next;
        func_name->value->accent = GKA_PARSE_DEF;
        func_name->is_head = 1;
        struct cell *func_cell = new_cell(new_cell_value_obj(stack_cell));
        ctx->stack = push_parse_stack(ctx->stack, func_cell, NULL);

        func_name->next = func_cell;

        resolve_next(ctx, func_name);
        ctx->cell = stack_cell;
        
        new->is_head = 1;
        ctx->next = new;

        printf("\x1b[36m");
        print_cell(stack_cell);
        printf("\x1b[0m\n");

    }else{
        ctx->stack = push_parse_stack(ctx->stack, ctx->cell, NULL);
        struct cell *blank = new_cell(new_value());
        resolve_next(ctx, ctx->next);
        ctx->cell = ctx->next;
        ctx->next = blank;
    }

    ctx->accent = GKA_PARSE_NO_ACCENT;
}

static void append_branch_cell(struct parse_ctx *ctx, struct cell *stack_cell, struct cell *new, struct cell *previous){
    previous->next = new;
    stack_cell->branch = previous;
    previous->is_head = 1;
    ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
    ctx->cell = stack_cell;
}

static void setup_branch(struct parse_ctx *ctx, struct cell *new){
    struct cell *stack_cell = new_cell(new_value());
    struct cell *next = ctx->next;

    if(debug){
        printf("setup branch: cell/new/next/stack %d ----", ctx->next->is_head);
        printf("\n");
        print_cell(ctx->cell);
        printf("\n");
        print_cell(new);
        printf("\n");
        print_cell(next);
        printf("\n");
        print_cell(stack_cell);
    }

    if(ctx->in_branching == 0){
        ctx->in_branching = 1;

        stack_cell->is_head = next->is_head;
        resolve_next(ctx, stack_cell);

        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        ctx->cell = stack_cell;

        next->is_head = 1;
        resolve_next(ctx, next);

        ctx->cell = next;
        ctx->next = new;
    }else{
        /*
        stack_cell->is_head = 1;
        resolve_next(ctx, stack_cell);

        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);
        ctx->cell = stack_cell;
        */

        ctx->next->is_head = 1;
        resolve_next(ctx, next);
    }

    if(debug){
        printf("\n----\n");
    }
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
    
    ctx->in_branching = 0;
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
                new->is_head = 0;
                parse_stack_count--;
            }else{
                fprintf(stderr, "parse below stack error\n");
                exit(1);
            }
            ctx->next_is_outof--;
            ctx->in_branching = 0;
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
            ctx->in_branching = 1;
        }
        while(ctx->next_is_into > 0){
            if(debug){
                printf("branch\n");
            }
            indent++;
            setup_branch(ctx, new);
            ctx->next_is_into--;
            ctx->in_branching = 1;
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
            struct cell *stack_cell = new_cell(new_value());
            ctx->cell = ctx->root = root; 
            stack_cell->is_head = 1;
            ctx->next = new;
            new->is_head = 1;
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
