enum STATES {
    START = 0,
    IN_CELL = 0,
    IN_STRING = 1
};

struct parse_ctx {
    struct cell *current;
    struct cell *root;
    enum STATES state;
    bool in_escape;
    struct string *current_name;
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

void parse_char(struct parse_ctx *ctx, char c){

    write(STDOUT, "-> ", 3);
    write(STDOUT, &c, 1);
    write(STDOUT, "\n", 1);

    struct cell *slot;
    struct symbol *symbol;

    if(ctx->state == START){
        if(c == '('){
            if(ctx->root == NULL){
                ctx->root = new_cell();

                if(ctx->root == NULL){
                    char msg[] = "Error allocating root cell aborting";
                    exit(1);
                }

                ctx->current = ctx-> root;
            }

            slot = ctx->current;
            ctx->current = new_cell();
            slot->next = ctx->current;;
        }
    }

    if(ctx->state == IN_STRING){
        if(c == '\\' && !ctx->in_escape){
            ctx->in_escape = 1;
            return;
        }

        if(!ctx->in_escape && c == '"'){
            ctx->state = START;
        }else{
            symbol = get_or_create_symbol(ctx->current);
            string_append_char(symbol->name, c);
        }
    }

    string_append_char(ctx->current_name, c);
}
