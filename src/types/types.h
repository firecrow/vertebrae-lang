/* string */
struct string *new_string();
struct string *clone_string(struct string *string);
struct string *string_free(struct string *string);
int _string_resize(struct string *string, size_t length);
int string_append(struct string *string, struct string *additional);
int string_append_char(struct string *string, char c);
struct string *string_from_cstring(char *cstring);
struct string *new_string_xprintf(const char *restrict format, ...);
#define str(XARG) string_from_cstring((XARG))

/* regex */
bool regex_match(char *pattern, struct string *source);

/* value_obj */
struct value_obj *new_value();
struct value_obj *value_if_type(struct value_obj *value, enum SL_TYPE type);
struct value_obj *clone_value(struct value_obj *value);
struct value_obj *value_from_token(enum SL_PARSE_STATE state, struct string *token);

/* tree */
struct tree *new_tree();
struct tree_entry *new_tree_entry();
struct value_obj *tree_get(struct tree *tree, struct string *key);
void tree_add(struct tree *tree, struct string *key, struct value_obj *value);
