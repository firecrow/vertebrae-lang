int super_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    if(c == '^'){
       ctx->accent = GKA_PARSE_SUPER;
       pattern->state = GKA_PARSE_DONE;
       return complete_previous(pattern, ctx);
    }
}
