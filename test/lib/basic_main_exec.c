#include "../../src/gekkota.h"

int main(){
    char *args[] = {"ls", "../", NULL};
    printf("hello world\n");
    gka_exec("ls", args);
    return 0;
}
