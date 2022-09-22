void quote_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    if(c == '\''){
    printf("setting parse\n");
       ctx->accent = GKA_PARSE_QUOTE;
       pattern->state = GKA_PARSE_DONE;
    }
}
