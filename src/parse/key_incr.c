int key_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    if(pattern->state == GKA_PARSE_NOT_STARTED && c == '.'){
        pattern->state = GKA_PARSE_IN_MATCH;
        return complete_previous(pattern, ctx);
    }
    if(pattern->state == GKA_PARSE_IN_MATCH){
        if(is_alphanum(c) || c == '-'){
            string_append_char(pattern->token, c);
            return complete_previous(pattern, ctx);
        }
    }
    if(c == '\0'){
        printf("token: %s\n", pattern->token->content);
        finalize(ctx, new_key_value_obj(pattern->token));
        pattern->token = new_string();
        pattern->state = GKA_PARSE_NOT_STARTED;
    }
    pattern->state = GKA_PARSE_NOT_STARTED;
    return complete_previous(pattern, ctx);
}
