int set_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    if(c == '^'){
        ctx->accent = GKA_PARSE_SET;
        pattern->state = GKA_PARSE_DONE;
        return complete_previous(pattern, ctx);
    }
    return 0;
}
