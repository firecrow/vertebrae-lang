
int symbol_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    /* note anything starting with a 0-9 will be caught by number first */
    if(
        (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_')
        || (pattern->state == GKA_PARSE_IN_MATCH && c == '-')
        || (pattern->state == GKA_PARSE_IN_MATCH && (c >= '0' && c <= '9'))
    ){
        string_append_char(pattern->token, c);
        pattern->state = GKA_PARSE_IN_MATCH;
    }else{
        if(pattern->state == GKA_PARSE_IN_MATCH){
            finalize(ctx, new_symbol_value_obj(pattern->token));
        }
        pattern->token = new_string();
        pattern->state = GKA_PARSE_NOT_STARTED;
    }
    return complete_previous(pattern, ctx);
}
