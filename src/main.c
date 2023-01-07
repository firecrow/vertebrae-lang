#include "gekkota.h"

int spacing = 0;
static int debug = 1;

int main(int argc, char *argv[]) {

    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);

    if(debug){
        print_branches(root, 0);
    }
    
    struct crw_state *state = crw_new_state_context();
    run_root(state, root);

    return 0;
}
