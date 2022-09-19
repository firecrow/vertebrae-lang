static enum match_state {
  GKA_PARSE_NOT_STARTED = 0,
  GKA_PARSE_PARTIAL,
  GKA_PARSE_IN_MATCH,
  GKA_PARSE_DONE
};

static typedef struct match_state (*pattern_incr_func)(struct cell_match_pattern *pattern, char c);

struct match_pattern {
  enum match_type type;  
  enum match_state state;
  bool in_escape;
  struct string token;
  pattern_incr_func incr; 
  void *data;
};

#define GKA_PATTERN_COUNT 9
#define GKA_PATTERN_START 0 
struct pattern_incr_func patterns[GKA_PATTERN_COUNT] = {
  string_incr
  open_cell
  close_cell
  quote_incr
  not_incr
  key_incr
  number_incr
  symbol_incr
};

struct parse_ctx {
    struct cell *current;
    struct cell *root;
    struct stack_item *stack;
    struct match_pattern
    struct match_pattern *current;
};

struct parse_ctx *new_parse_ctx();
struct cell *parse_all(struct string *string);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);

