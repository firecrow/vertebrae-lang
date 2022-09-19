#include "../gekkota.h"

#include "parse.h"

#include "close_cell_incr.c"
#include "key_incr.c"
#include "not_incr.c"
#include "open_cell_incr.c"
#include "quote_incr.c"
#include "string_incr.c"
#include "super_incr.c"
#include "symbol_incr.c"

struct parse_ctx *new_parse_ctx(){
    struct parse_ctx *ctx = malloc(sizeof(struct parse_ctx));

    if(ctx == NULL){
        return NULL;
    }
    memset(ctx, 0, sizeof(struct parse_ctx));
    return ctx;
}


void setup_parse_ctx(struct parse_ctx *ctx){
  for(int i = 0; i< GKA_PATTERN_COUNT; i++){
    struct match_pattern *pattern = malloc(sizeof(struct match_pattern));
    memset(pattern, 0, sizeof(struct match_pattern));
    pattern->incr = patterns[i];
    ctx->patterns[i] = pattern; 
  }
}

static struct stack_item *push_parse_stack(struct stack_item *existing, struct cell *cell, struct head *head){
    struct stack_item *item = new_stack_item(existing, cell, head);
    return item;
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

bool is_whitespace(char c){
    return c == ' ' || c == '\t' || c == '\n';
}

void parse_char(struct parse_ctx *ctx, char c){
  int pattern_idx = GKA_PATTERN_START;
  struct match_pattern *pattern = NULL;
  enum match_state result = GKA_PARSE_NOT_STARTED;

  ctx->current->incr(ctx->currnet, ctx, c);

  if(ctx->current->state != GKA_PARSE_IN_MATCH){
    while(pattern_idx < GKA_PATTERN_COUNT){
      pattern = ctx->patterns[patter_idx++];
      current->incr(current, ctx, c);
      if(pattern->state > GKA_PARTIAL){
        ctx->pattern = pattern;
        break;
      }
    }
  }
}
