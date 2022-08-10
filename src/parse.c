enum STATES {
    START = 0,
    IN_CELL = 0,
    IN_STRING = 1
};

struct parse_ctx {
    struct cell *current;
    struct cell *root;
    enum STATES state;
    enum STATES prev_state;
    bool in_escape;
    struct string *token;
};

void parse_char(struct parse_ctx *ctx, char c);

struct parse_ctx *new_parse_ctx(){
    struct parse_ctx *ctx = malloc(sizeof(struct parse_ctx));

    if(ctx == NULL){
        return NULL;
    }
    memset(ctx, 0, sizeof(struct parse_ctx));
    return ctx;
}


struct cell *parse_all(struct string *string){
    struct parse_ctx *ctx = new_parse_ctx();

    if(ctx == NULL){
        char msg[] = "Error allocating parse context, aborting";
        write(STDERR, msg, strlen(msg));
        exit(1);
    }

    int l = string->length;
    for(int i = 0; i<l; i++){
       parse_char(ctx, string->content[i]);
    }

    return ctx->root; 
}

struct cell*parse_file(int fd){
    char buffer[1];

    struct parse_ctx *ctx = new_parse_ctx();
    if(ctx == NULL){
        char msg[] = "Error allocating parse context, aborting";
        write(STDERR, msg, strlen(msg));
        exit(1);
    }
    
    while(read(fd, buffer, 1) > 0){
       parse_char(ctx, buffer[0]);
    }
    return ctx->root; 
}

void assign_cell_attributes(struct cell *cell, struct string *token){
    cell->value = value_from_token(token);
}

void finalize_cell(struct parse_ctx *ctx){
    if(ctx->token){
        assign_cell_attributes(ctx->current, ctx->token);

        string_free(ctx->token);

        ctx->token = NULL;
    }

    ctx->state = START;
    if(ctx->state != START){
        ctx->prev_state = ctx->state;
    }
}

void parse_char(struct parse_ctx *ctx, char c){

    write(STDOUT, "-> ", 3);
    write(STDOUT, &c, 1);
    write(STDOUT, "\n", 1);

    struct cell *slot;
    struct cell *new;
    struct symbol *symbol;

    if(ctx->state == IN_STRING){
        if(c == '\\' && !ctx->in_escape){
            ctx->in_escape = 1;
            return;
        }

        if(!ctx->in_escape && c == '"'){
            ctx->state = START;
            finalize_cell(ctx);
            return;
        }else{
            symbol = get_or_create_symbol(ctx->current);
            string_append_char(symbol->name, c);
        }

        return;
    }

    if(c == '('){
        finalize_cell(ctx);

        new = new_cell();
        if(new == NULL){
            if(new == NULL){
                char msg[] = "Error allocating root cell aborting";
                exit(1);
            }

        }

        slot = ctx->current;
        if(!ctx->root){
            ctx->root = new;
        }else{
            slot->branch = new;
        }

        ctx->state = IN_CELL;
        return;
    }

    if(c == ')'){
        finalize_cell(ctx);
        return;
    }

    if(c == ' ' || c == '\t' || c == '\n'){
        finalize_cell(ctx);
        return;
    }

    if(ctx->state == START){
        new = new_cell();
        if(new == NULL){
            char msg[] = "Error allocating root cell aborting";
            exit(1);
        }

        slot = ctx->current;
        ctx->current = new;
        if(!slot){
            ctx->root = new;
        }else{
            slot->next = new;
        }
        ctx->state = IN_CELL;
    }

    string_append_char(ctx->token, c);
}
