void string_incr(struct cell_match_pattern *pattern, char c){
    if(pattern->state == GKA_NOT_STARTED){
      if(c == '"'){
        pattern->state = GKA_PARSE_IN_MATCH;
        return;
      }
    }
    if(pattern->state == IN_MATCH){
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
            ctx->slot = ctx->current;
            ctx->current = new_string_value_obj(pattern->token);
            ctx->slot->next = ctx->current;
        }else{
            string_append_char(pattern->token, c);
        }
    }
}
