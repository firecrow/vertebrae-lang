#include "external.h"

#include "ssimple.h"
#include "core/core.h"
#include "types/types.h"
#include "run/run.h"
#include "operators/operator.h"
#include "library/library.h"

int spacing = 0;

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct head *head = new_head();
    struct cell *root = parse_file(source);
    struct closure *global = new_closure(NULL);
    struct stack_item *stack = new_stack_item(NULL, root, head);

    init_basic_library(global); 
    struct crw_state *ctx = crw_new_state_context(root, global, stack);

    while(!ctx->status == CRW_DONE){
       ctx->next(ctx); 
    }

    return 0;
}
