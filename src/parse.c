struct parse_ctx {
    struct cell *current;
    struct cell *root;
};

struct parse_ctx *new_parse_ctx(){
    struct parse_ctx *ctx = malloc(sizeof(struct parse_ctx));

    if(ctx == NULL){
        return NULL;
    }
    return ctx;
}

void parse_char(struct parse_ctx *ctx, char c){
    write(STDOUT, "-> ", 3);
    write(STDOUT, &c, 1);
    write(STDOUT, "\n", 1);
}

struct cell *parse_all(struct string *str){
    struct parse_ctx *ctx = new_parse_ctx();

    if(ctx == NULL){
        char msg[] = "Error allocating parse context, aborting";
        write(STDERR, msg, strlen(msg));
        exit(1);
    }

    int l = str->length;
    for(int i = 0; i<l; i++){
       parse_char(ctx, str->content[i]);
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
