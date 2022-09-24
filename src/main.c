#include "gekkota.h"

int spacing = 0;

int main(int argc, char *argv[]) {

    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);
    
    struct crw_state *state = crw_new_state_context();
    run_root(state, root);

    return 0;
}
