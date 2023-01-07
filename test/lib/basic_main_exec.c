#include "../../src/gekkota.h"

int main(){
    char *args[] = {"tail", "-f", "/var/log/mpd/mpd.log", NULL};
    printf("hello world\n");
    gka_exec("tail", args);
    return 0;
}
