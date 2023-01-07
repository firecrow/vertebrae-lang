enum match_state {
  GKA_PARSE_NOT_STARTED = 0,
  GKA_PARSE_PARTIAL,
  GKA_PARSE_IN_MATCH,
  GKA_PARSE_DONE
};

struct match_pattern;
struct parse_ctx;

typedef int (*pattern_incr_func)(struct match_pattern *pattern, struct parse_ctx *ctx, char c);

struct match_pattern {
    enum match_state state;
    bool in_escape;
    struct string *token;
    pattern_incr_func incr; 
    void *data;
};

#define GKA_PATTERN_START 0 
#define GKA_PATTERN_COUNT 11

struct parse_ctx {
    struct cell *cell;
    struct cell *next;
    struct cell *root;
    struct cell *previous;
    struct cell *grand_previous;
    bool next_is_branch;
    bool next_is_into;
    bool next_is_outof;
    bool next_func_into;
    bool in_branching;
    enum parse_accent accent;
    struct stack_item *stack;
    struct match_pattern *current;
    struct match_pattern *patterns[GKA_PATTERN_COUNT];
};

struct parse_ctx *new_parse_ctx();
struct cell *parse_all(char *script);
struct cell *parse_file(int fd);
void parse_char(struct parse_ctx *ctx, char c);

bool is_whitespace(char c);
bool is_alphanum(char c);
bool is_allnum(char c);

struct parse_ctx *new_parse_ctx();
static struct match_pattern *setup_pattern(pattern_incr_func func);
void setup_parse_ctx(struct parse_ctx *ctx);
static struct stack_item *push_parse_stack(struct stack_item *existing, struct cell *cell, struct head *head);
static int complete_previous(struct match_pattern *pattern, struct parse_ctx *ctx);
void finalize_parse(struct parse_ctx *ctx);
