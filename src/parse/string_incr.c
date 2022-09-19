void string_incr(struct match_pattern *pattern, struct parse_ctx *ctx, char c){
    printf("string iter...\n");
    struct cell *slot;
    if(pattern->state == GKA_PARSE_NOT_STARTED){
      if(c == '"'){
        pattern->state = GKA_PARSE_IN_MATCH;
        return;
      }
    }
    if(pattern->state == GKA_PARSE_IN_MATCH){
        if(c == '\\'){
            /* if we are escaping the \ then take no action */
            if(pattern->in_escape){
                pattern->in_escape = 0;
            }else{
                pattern->in_escape = 1;
                return;
            }
        }
        if(pattern->in_escape){
            pattern->in_escape = 0;
            if(c == 'n'){
                c = '\n';
            }
            if(c == 't'){
                c = '\t';
            }
        }

        if(!pattern->in_escape && c == '"'){
            pattern->state = GKA_PARSE_DONE;
            slot = ctx->cell;
            ctx->cell = new_cell(new_string_value_obj(pattern->token));
            slot->next = ctx->cell;
        }else{
            string_append_char(pattern->token, c);
        }
    }
}
