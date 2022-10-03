#include "../src/gekkota.h"

#include "suite.c"

/*
#include "basic_steps.c"
*/

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
#include "pop_stack.c"
#include "arithmetic_test.c"
#include "variable_test.c"
#include "function_test.c"
#include "parse_test.c"
#include "swap_symbol_test.c"

int main(){
    test_tree();
    test_head();
    test_steps();
    test_basic();
    test_pop_stack();
    test_arithmetic();
    test_variables();
    test_functions();
    test_parse();
    test_swap_symbol();




    /********************************* Basic run test ********************/
    suite = new_suite("Run tests");

    script = "(+ 1 3 5)";

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->head->value->type == SL_TYPE_INT, "head value is int");
    test(suite, state->head->value->slot.integer == 9, "arithmetic valu is the cntent of the cells");

    script = "(.hi \"there\" (mock))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    value = state->head->value;
    print_value(value);

    test(suite, value->type == SL_TYPE_STRING, "returned is an int");
    test(suite, string_cmp(value->slot.string, str("there")) == 0, "hi key has 'there' value");

    summerize(suite);
    /********************************* Function run test ********************/
    suite = new_suite("Function tests");

    script = "(\n    .hi \"there\"\n    .func '(\n        print hi))";
    printf("%s\n", script);

    root = parse_all(script);
    printf("-----------------------------\n");
    fflush(stdout);
    state = crw_new_state_context();
    run_root(state, root);

    printf("no print runs");
    test(suite, state->context == NULL, "mock never runs");


    script = "(\n    .hi \"there\"\n    .func '(\n        print hi)\n    (func))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    /*
    if(state->context){
        value = swap_for_symbol(state->context->closure, new_symbol_value_obj(str("hi")));
    }else{
        value = NULL;
    }
    print_head(state->context);

    test(suite, value && value->type == SL_TYPE_STRING, "returned is an string");
    test(suite, value && string_cmp(value->slot.string, str("there")) == 0, "after function call mock has 'there' value");
    */

    script = "(\n    .hi \"there\"\n    .func '(\n        mock (print \"hello, \" value))\n    (func \"one\" \"two\" \"three\"))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    if(state->context){
        value = swap_for_symbol(state->context->closure, new_symbol_value_obj(str("hi")));
    }else{
        value = NULL;
    }

    /*
    test(suite, value && value->type == SL_TYPE_STRING, "returned is an string II");
    test(suite, value && string_cmp(value->slot.string, str("there")) == 0, "after function call mock has 'there' value II");
    */


    summerize(suite);

    show_global_success();

    return 0;
}
