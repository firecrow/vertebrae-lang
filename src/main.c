#include "external.h"

#include "ssimple.h"
#include "core/core.h"
#include "types/types.h"
#include "operators/operator.h"
#include "library/library.h"

int spacing = 0;

void print_space(){
    for(int i =0; i< spacing; i++){
        write(STDOUT, " ", 1);
    }
}

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);
    struct closure *global = new_closure(NULL);
    struct stack_item *stack = new_stack_item();

    init_basic_library(global); 

    struct clsp_state *state = new_clsp_state(root, global, stack);

    while(!state->status == CRW_STATUS_DONE)){
       state->next(state); 
    }

    return 0;
}
