#include "../gekkota.h"

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

/* ----- string ----- */
void string_incr(struct cell_match_pattern *pattern, char c){
    if(pattern->state == GKA_NOT_STARTED){
      if(c == '"'){
        pattern->state = GKA_PARSE_IN_MATCH;
        return;
      }
    }
    if(pattern->state == IN_MATCH){
        if(c == '\\'){
            /* if we are escaping the \ then take no action */
            if(pattern->in_escape){
                pattern->in_escape = 0;
            }else{
                pattern->in_escape = 1;
                return;
            }
        }
        if(pattern->in_escape){
            pattern->in_escape = 0;
            if(c == 'n'){
                c = '\n';
            }
            if(c == 't'){
                c = '\t';
            }
        }

        if(!pattern->in_escape && c == '"'){
            pattern->state = GKA_PARSE_DONE;
            ctx->slot = ctx->current;
            ctx->current = new_string_value_obj(pattern->token);
            ctx->slot->next = ctx->current;
        }else{
            string_append_char(pattern->token, c);
        }
    }
}
/* ----- open cell ----- */
void open_incr(struct cell_match_pattern *pattern, char c){
  if(c == '('){
    new = new_cell();
    if(ctx->accent == GKA_PARSE_IN_QUOTE){
        ctx->current->value = new_cell_value_obj(new);
        ctx->stack = push_parse_stack(ctx->stack, ctx->current, NULL);
        ctx->current = new;
    }else{
        stack_cell = new_cell();
        stack_cell->branch = new;

        slot = ctx->current;

        ctx->current = new;
        ctx->stack = push_parse_stack(ctx->stack, stack_cell, NULL);

        if(!ctx->root){
            ctx->root = stack_cell;
        }else{
            if(slot){
                slot->next = stack_cell;
            }
        }
    }
    pattern->state = GKA_PARSE_DONE;
  }
}
/* ----- close cell ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
    if(c == '('){
      if(ctx->stack){
          ctx->current = ctx->stack->cell;
          ctx->stack = ctx->stack->previous;
      }else{
          ctx->current = NULL;
      }
      pattern->state = GKA_PARSE_DONE;
    }
}
/* ----- quote ' ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
    if(c == '\''){
       ctx->accent = GKA_ACCENT_QUOTE
       pattern->accent = GKA_PARSE_DONE;
    }
}
/* ----- not ! ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
    if(c == '!'){
       ctx->accent = GKA_ACCENT_NOT;
       pattern->state = GKA_PARSE_DONE;
    }
}
/* ----- super ^ ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
    if(c == '^'){
       ctx->accent = GKA_ACCENT_SUPER;
       pattern->state = GKA_PARSE_DONE;
    }
}
/* ----- key ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
  if(pattern->state == GKA_NOT_STARTED && c == '.'){
    pattern->state = GKA_IN_MATCH;
    return;
  }
  if(pattern->state == GKA_IN_MATCH){
    if(is_alphanum(c) || c == '-'){
      string_append_char(pattern->token, c);
    }else{
      pattern->state = GKA_NOT_STARTED;
    }
  }
}
/* ----- number ----- */
void close_incr(struct cell_match_pattern *pattern, char c){
  if(pattern->state == GKA_NOT_STARTED && c == '-' || is_num(c)){
    string_append_char(pattern->token, c);
  }else{
    if(pattern->token->length){
       int value = atoi(pattern->token);
       ctx->add_new_cell(new_int_value_obj(value));
    }
    pattern->state = GKA_NOT_STARTED;
  }
}
/* ----- symbol ----- */
void symbol_incr(struct cell_match_pattern *pattern, char c){
  if(c != '\0'){
    string_append_char(pattern->token, c);
  }else{
    ctx->add_string_cell(new_symbol_value_obj(pattern->token));
    pattern->state = GKA_NOT_STARTED;
  }
}
