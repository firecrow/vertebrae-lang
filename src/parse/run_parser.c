#include "../gekkota.h"

static int debug = 1;

static int indent = 0;
static void print_space(){
    int in = indent*4;
    while(in-- > 0){
        printf(" ");
    }
}

int parse_stack_count = 0;

#include "parse_utils.c"

#include "close_cell_incr.c"
#include "whitespace_incr.c"
#include "key_incr.c"
#include "number_incr.c"
#include "not_incr.c"
#include "open_cell_incr.c"
#include "quote_incr.c"
#include "string_incr.c"
#include "symbol_incr.c"
#include "into_incr.c"
#include "into_func_incr.c"
#include "def_incr.c"
#include "set_incr.c"


struct parse_ctx *new_parse_ctx(){
    indent = 0;
    struct parse_ctx *ctx = malloc(sizeof(struct parse_ctx));

    if(ctx == NULL){
        return NULL;
    }
    memset(ctx, 0, sizeof(struct parse_ctx));
    return ctx;
}

static struct match_pattern *setup_pattern(pattern_incr_func func){
    struct match_pattern *pattern = malloc(sizeof(struct match_pattern));
    memset(pattern, 0, sizeof(struct match_pattern));
    pattern->incr = func;
    pattern->token = new_string();
    return pattern;
}

struct match_pattern *close_pattern;

void setup_parse_ctx(struct parse_ctx *ctx){

  int i = 0;
  ctx->patterns[i++] = setup_pattern(string_incr);
  ctx->patterns[i++] = setup_pattern(number_incr);
  ctx->patterns[i++] = setup_pattern(def_incr);
  ctx->patterns[i++] = setup_pattern(set_incr);
  ctx->patterns[i++] = setup_pattern(symbol_incr);
  ctx->patterns[i++] = setup_pattern(into_incr);
  ctx->patterns[i++] = setup_pattern(into_func_incr);
  ctx->patterns[i++] = setup_pattern(whitespace_incr);
  ctx->patterns[i++] = setup_pattern(close_cell_incr);
  /*
  ctx->patterns[i++] = setup_pattern(not_incr);
  */
  ctx->patterns[i++] = NULL;
}

static struct stack_item *push_parse_stack(struct stack_item *existing, struct cell *cell, struct head *head){
    parse_stack_count++;
    struct stack_item *item = new_stack_item(existing, cell, head);
    return item;
}

struct cell *parse_all(char *script){
    struct parse_ctx *ctx = new_parse_ctx();

    if(ctx == NULL){
        char msg[] = "Error allocating parse context, aborting";
        write(STDERR, msg, strlen(msg));
        exit(1);
    }

    setup_parse_ctx(ctx);

    char *p = script;
    while(*p != '\0'){
       parse_char(ctx, *p++);
    }
    finalize_parse(ctx);

    return ctx->root; 
}

struct cell *parse_file(int fd){
    char buffer[1];

    struct parse_ctx *ctx = new_parse_ctx();
    if(ctx == NULL){
        char msg[] = "Error allocating parse context, aborting";
        write(STDERR, msg, strlen(msg));
        exit(1);
    }

    setup_parse_ctx(ctx);
    
    while(read(fd, buffer, 1) > 0){
       parse_char(ctx, buffer[0]);
    } return ctx->root; 
}

void parse_char(struct parse_ctx *ctx, char c){
    struct match_pattern *pattern = NULL;
    int idx = 0;
    while((pattern = ctx->patterns[idx++])){
       if( 0 && debug){
           printf("idx:%c:%d\n",c, idx-1);
       }
       
       fflush(stdout);

       if(pattern->incr(pattern, ctx, c)){
            fflush(stdout);
            break;
       }
       fflush(stdout);
    }
}
