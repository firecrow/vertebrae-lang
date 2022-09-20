void symbol_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    if(c == '\0'){
        finalize(ctx, new_symbol_value_obj(pattern->token)); 
        pattern->token = new_string();
        pattern->state = GKA_PARSE_NOT_STARTED;
        return;
    }
    string_append_char(pattern->token, c);
    pattern->state = GKA_PARSE_IN_MATCH;
}
