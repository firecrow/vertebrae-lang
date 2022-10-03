char NUETRAL[] = "\x1b[0m";
char RED[] = "\x1b[31m";
char GREEN[] = "\x1b[32m";

struct suite {
    char *name;
    int passed;
    int failed;
    int pass;
};

int global_open_count = 0;

static struct suite *new_suite(char *name){
    struct suite *suite = malloc(sizeof(struct suite));
    memset(suite, 0, sizeof(struct suite));
    suite->name = name;
    printf("SUITE ************ %s ************\n", name);

    global_open_count++;
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
        global_open_count--;
        printf("%sSUITE PASS:", GREEN);
    }else{
        printf("%sSUITE FAIL:", RED);
    }
    printf(" *%s* %d PASSED, %d FAILED %s%s\n", suite->name, suite->passed, suite->failed, suite->name, NUETRAL);
}

static void show_global_success(){
    if(!global_open_count){
        printf("%sALL TESTS PASS%s\n", GREEN, NUETRAL);
    }else {
        printf("%sTEST FAILURES: %d%s\n", RED, global_open_count, NUETRAL);
    }
}

