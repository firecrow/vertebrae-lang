#include "../gekkota.h"
#include "parse.h"

void not_incr(struct cell_match_pattern *pattern, char c){
    if(c == '!'){
       ctx->accent = GKA_ACCENT_NOT;
       pattern->state = GKA_PARSE_DONE;
    }
}
