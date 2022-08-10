#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <fcntl.h>

#include "ssimple.h"
#include "regex.c"
#include "value.c"
#include "string.c"
#include "symbol.c"
#include "cell.c"
#include "parse.c"

#include "debug.c"

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);
    char msg[] = "Root is null?";

    printf("Root is null? %d\n", root == NULL);

    struct cell *cell = root;
    while(cell){
        print_cell(cell);
        if(cell->branch){
            cell = cell->branch;
            printf("branch\n");
        } else {
            printf("cell\n");
            cell = cell->next;
        }
    }
    return 0;
}
