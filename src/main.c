#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define STDIN 0 
#define STDOUT 1
#define STDERR 2 

#include "ssimple.h"
#include "parse.c"

int main() {
    struct cell *root = parse_file(STDIN);
    return 0;
}
