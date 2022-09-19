enum match_state {
  GKA_PARSE_NOT_STARTED = 0,
  GKA_PARSE_PARTIAL,
  GKA_PARSE_IN_MATCH,
  GKA_PARSE_DONE
};

enum parse_accent {
  GKA_PARSE_NO_ACCENT,
  GKA_PARSE_QUOTE,
  GKA_PARSE_SUPER,
  GKA_PARSE_DOT,
  GKA_PARSE_NOT
};

struct match_pattern;
struct parse_ctx;

typedef void (*pattern_incr_func)(struct match_pattern *pattern, struct parse_ctx *ctx, char c);

struct match_pattern {
  enum match_state state;
  bool in_escape;
  struct string *token;
  pattern_incr_func incr; 
  void *data;
};

#define GKA_PATTERN_START 0 
#define GKA_PATTERN_COUNT 10

struct parse_ctx {
    struct cell *cell;
    struct cell *root;
    enum parse_accent accent;
    struct stack_item *stack;
    struct match_pattern *current;
    struct match_pattern *patterns[GKA_PATTERN_COUNT];
};

struct parse_ctx *new_parse_ctx();
struct cell *parse_all(struct string *string);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);

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

struct parse_ctx *new_parse_ctx();
static struct match_pattern *setup_pattern(pattern_incr_func func);
void setup_parse_ctx(struct parse_ctx *ctx);
static struct stack_item *push_parse_stack(struct stack_item *existing, struct cell *cell, struct head *head);
struct cell *parse_all(struct string *string);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);
