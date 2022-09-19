#include "../gekkota.h"
#include "parse.h"

void quote_incr(struct cell_match_pattern *pattern, char c){
    if(c == '\''){
       ctx->accent = GKA_ACCENT_QUOTE
       pattern->accent = GKA_PARSE_DONE;
    }
}
