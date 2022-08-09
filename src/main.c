#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ssimple.h"
#include "string.c"
#include "symbol.c"
#include "cell.c"
#include "parse.c"

#include "debug.c"

int main() {
    struct cell *root = parse_file(STDIN);
    char msg[] = "Root is null?";

    printf("Root is null? %d\n", root == NULL);

    struct cell *cell = root;
    do {
        print_cell(cell);
    }while(cell = cell->next);
    return 0;
}
