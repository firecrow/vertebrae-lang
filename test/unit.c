#include "../src/gekkota.h"

#include "suite.c"

struct suite *suite = NULL;
struct order_entry *order_entry = NULL;
struct value_obj *result = NULL;
struct crw_state *state = NULL;

struct head *head = NULL;
struct cell *cell = NULL;
struct closure *closure = NULL;
struct value_obj *value = NULL;
struct operator_ifc *op;
struct string *op_name;
char *script = NULL;

struct closure *global = NULL;
struct stack_item *stack = NULL;
struct cell *root = NULL;
struct cell *branch = NULL;
struct cell *first = NULL;
struct cell *second = NULL;
struct cell *third = NULL;
struct cell *fourth = NULL;
struct cell *fifth = NULL;
struct cell *sixth = NULL;
struct cell *seventh = NULL;
struct cell *eigth = NULL;
struct cell *ninth = NULL;
struct cell *tenth = NULL;
struct cell *eleventh = NULL;

#include "tree_test.c"
#include "head_test.c"
#include "step_test.c"
#include "basic_run_test.c"
#include "basic_steps.c"
#include "pop_stack.c"
#include "arithmetic_test.c"
#include "variable_test.c"
#include "function_test.c"
#include "function_run_test.c"
#include "parse_test.c"
#include "swap_symbol_test.c"

int main(){
    test_tree();
    test_head();
    test_steps();
    test_basic();
    test_pop_stack();
    test_parse();
    test_arithmetic();
    test_variables();
    test_functions();
    test_swap_symbol();
    test_basic_run();
    test_run_functions();

    show_global_success();

    return 0;
}
