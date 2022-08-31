#include "../src/external.h"

char NUETRAL[] = "\x1b[0m";
char RED[] = "\x1b[31m";
char GREEN[] = "\x1b[32m";

struct suite {
    char *name;
    int passed;
    int failed;
    int pass;
};

static struct suite *new_suite(char *name){
    struct suite *suite = malloc(sizeof(struct suite));
    memset(suite, 0, sizeof(struct suite));
    suite->name = name;
    return suite;
}

static void test(struct suite *suite, int pass, char *msg){
    char *color = NUETRAL;
    if(pass){
        suite->passed++;
        color = GREEN;
    }else{
        suite->failed++;
        color = RED;
    }
    printf("%sTEST: %s %s%s\n", color, pass ? "PASS" : "FAIL", msg, NUETRAL);
}

static void summerize(struct suite *suite){
    if(suite->failed <= 0){
        suite->pass = 1;
    }
    if(suite->pass){
        suite->passed++;
        printf("%sSUITE PASS:", GREEN);
    }else{
        printf("%sSUITE FAIL:", RED);
    }
    printf("%d PASSED, %d FAILED %s%s\n", suite->passed, suite->failed, suite->name, NUETRAL);
}

struct suite *suite = NULL;
int main(){

    /***************** TEST SUITE *************/
    suite = new_suite("Test Suite");
    test(suite, 1, "Testing success");
    test(suite, 0, "Testing failure");
    summerize(suite);
    return 0;
}
