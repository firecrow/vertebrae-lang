#include "../gekkota.h"
#include "parse.h"

void super_incr(struct cell_match_pattern *pattern, char c){
    if(c == '^'){
       ctx->accent = GKA_ACCENT_SUPER;
       pattern->state = GKA_PARSE_DONE;
    }
}
