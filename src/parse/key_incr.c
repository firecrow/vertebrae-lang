#include "../gekkota.h"
#include "parse.h"

void key_incr(struct cell_match_pattern *pattern, char c){
  if(pattern->state == GKA_NOT_STARTED && c == '.'){
    pattern->state = GKA_IN_MATCH;
    return;
  }
  if(pattern->state == GKA_IN_MATCH){
    if(is_alphanum(c) || c == '-'){
      string_append_char(pattern->token, c);
    }else{
      pattern->state = GKA_NOT_STARTED;
    }
  }
}
