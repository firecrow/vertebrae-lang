#include "gekkota.h"

int spacing = 0;

int main(int argc, char *argv[]) {

    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);
    
    run_root(root);

    return 0;
}
