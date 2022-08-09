#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "ssimple.h"
#include "string.c"
#include "symbol.c"
#include "cell.c"
#include "parse.c"

int main() {
    struct cell *root = parse_file(STDIN);
    return 0;
}
