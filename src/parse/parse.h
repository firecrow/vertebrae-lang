static enum match_state {
  GKA_PARSE_NOT_STARTED = 0,
  GKA_PARSE_PARTIAL,
  GKA_PARSE_IN_MATCH,
  GKA_PARSE_DONE
};

struct match_pattern;

typedef enum match_state (*pattern_incr_func)(struct cell_match_pattern *pattern, char c);

struct match_pattern {
  enum match_state state;
  bool in_escape;
  struct string token;
  pattern_incr_func incr; 
  void *data;
};

struct match_pattern lose_cell_incr;
struct match_pattern key_incr;
struct match_pattern not_incr;
struct match_pattern open_cell_incr;
struct match_pattern close_cell_incr;
struct match_pattern quote_incr;
struct match_pattern number_incr;
struct match_pattern string_incr;
struct match_pattern super_incr;
struct match_pattern symbol_incr;

#define GKA_PATTERN_COUNT 9
#define GKA_PATTERN_START 0 
pattern_incr_func patterns[GKA_PATTERN_COUNT] = {
  string_incr,
  open_cell_incr,
  close_cell_incr,
  quote_incr,
  not_incr,
  key_incr,
  number_incr,
  symbol_incr
};

struct parse_ctx {
    struct cell *current;
    struct cell *root;
    struct stack_item *stack;
    struct match_pattern *current;
};

struct parse_ctx *new_parse_ctx();
struct cell *parse_all(struct string *string);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);
