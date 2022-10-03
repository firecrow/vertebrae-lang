#include "../src/gekkota.h"

#include "suite.c"

/*
#include "arithmetic_test.c"
#include "basic_steps.c"
#include "function_run_test.c"
#include "function_test.c"
#include "parse_test.c"
#include "pop_stack.c"
#include "swap_symbol_test.c"
#include "unit.c"
#include "variable_test.c"
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

int main(){
    test_tree();
    test_head();
    test_steps();
    test_basic();



    /********************************* test pop_stack ********************/
    suite = new_suite("Pop stack branch tests");

    root = new_cell(NULL);
    root->value = new_string_value_obj(str("root"));

    first = new_cell(NULL);
    first->value = new_string_value_obj(str("first"));

    second = new_cell(NULL);
    second->value = new_string_value_obj(str("second"));

    third = new_cell(NULL);
    third->value = new_string_value_obj(str("third"));

    fourth = new_cell(NULL);
    fourth->value = new_string_value_obj(str("fourth"));
    
    fifth = new_cell(NULL);
    fifth->value = new_string_value_obj(str("fifth"));

    sixth = new_cell(NULL);
    sixth->value = new_string_value_obj(str("sixth"));

    /* 
     * ("first" "second" ("third" fourth") "fifth" "sixth")
     */
    root->next = first;
    first->next = second;
    second->next = fifth;
        second->branch = third;
        third->next = fourth;
    fifth->next = sixth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    state->next(state);
    test(suite, state->cell == first, "First cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == second, "Second cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == third, "Third step is the next after the branch cell(fourth)");

    state->next(state);
    test(suite, state->cell == fourth, "Fourth step is the next after the branch cell(fourth)");

    state->next(state);
    test(suite, state->cell == fifth, "Fifth cell should be on main path again (Third)");

    state->next(state);
    test(suite, state->cell == sixth, "Sixth cell should be on main path as well (Fifth)");

    summerize(suite);

    /********************************* test arithmetic ********************/
    suite = new_suite("Arithmetic tests");

    global = new_closure(NULL);
    init_basic_library(global); 

    root = new_cell(NULL);
    root->value = new_result_value_obj(NIL);

    branch = new_cell(NULL);
    branch->value = new_symbol_value_obj(str("+"));

    second = new_cell(NULL);
    second->value = new_int_value_obj(1);

    third = new_cell(NULL);
    third->value = new_int_value_obj(3);

    fourth = new_cell(NULL);
    fourth->value = new_int_value_obj(-2);

    root->branch = branch;

    branch->next = second;
    second->next = third;
    third->next = fourth;

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    /*
     * (+ 1 3 -2) 
     */

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    test(suite, state->head->value->slot.integer == 2, "Arithemtic comes up with proper value");

    summerize(suite);

    /********************************* Variable Assignment Tests ********************/
    suite = new_suite("Variable assignment tests");

    root = new_cell(NULL);
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell(NULL);
    root->value = new_symbol_value_obj(str("let"));

    second = new_cell(NULL);
    second->value = new_key_value_obj(str("one"));

    third = new_cell(NULL);
    third->value = new_int_value_obj(1);

    fourth = new_cell(NULL);

    fifth = new_cell(NULL);
    fifth->value = new_symbol_value_obj(str("+"));

    sixth = new_cell(NULL);
    sixth->value = new_int_value_obj(2);

    seventh = new_cell(NULL);
    seventh->value = new_symbol_value_obj(str("one"));

    root->next = second;
    second->next = third;
    third->next = fourth;
    fourth->branch = fifth;
    fifth->next = sixth;
    sixth->next = seventh;
    
    /* 
     * (let .one 1 (+ 2 one))
     */

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    while(state->status != CRW_DONE){
       state->next(state);
    }

    test(suite, tree_get(state->head->closure->symbols, str("one"))->slot.integer = 1,"Variable one is set with value 1");
    test(suite, state->head->value->slot.integer == 3, "Final value reflects variable value");

    summerize(suite);
    
    /********************************* Function pointer test ********************/
    suite = new_suite("Function test");

    /*
    root = new_cell(NULL);
    first = new_cell(NULL);
    global = new_closure(NULL);

    root = new_cell(NULL);
    root->value = new_symbol_value_obj(str("let"));
    root->id = 1;

    second = new_cell(NULL);
    second->value = new_key_value_obj(str("show"));
    second->id = 2;

        fourth = new_cell(NULL);
        fourth->value = new_symbol_value_obj(str("print"));
        fourth->id = 4;

        fifth = new_cell(NULL);
        fifth->value = new_symbol_value_obj(str("message"));
        fifth->id = 5;

    third = new_cell(NULL);
    third->id = 3;

    sixth = new_cell(NULL);
    sixth->id = 6;

    seventh = new_cell(NULL);
    seventh->value = new_symbol_value_obj(str("show"));
    seventh->id = 7;

    eigth = new_cell(NULL);
    eigth->value = new_key_value_obj(str("message"));
    eigth->id = 8;

    ninth = new_cell(NULL);
    ninth->value = new_string_value_obj(str("hello, crow-lisp"));
    ninth->id = 8;

    root->next = first;
    first->next = second;
    second->next = third;
    third->branch = fourth;
    fourth->next = fifth;
    third->next = sixth;
    sixth->branch = seventh;
    seventh->next = eigth;
    eigth->next = ninth;

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    */
    summerize(suite);

    /********************************* Parse test ********************/
    suite = new_suite("Parse tests");
    char *script = "(+ 127)";

    root = parse_all(script);

    /*
    printf("root: ");
    print_cell(root);
    printf("root->branch: ");
    print_cell(root->branch);
    printf("root->next: ");
    print_cell(root->next);
    printf("\n");
    */

    test(suite, root->branch->value->type == SL_TYPE_SYMBOL, "+ is symbol");
    test(suite, string_cmp(root->branch->value->slot.string, str("+")) == 0, "+ is the content of the symbol");
    test(suite, root->branch->next->value->type == SL_TYPE_INT, "1 is an int");
    test(suite, root->branch->next->value->slot.integer == 127, "1 is 127");

    script = "(let .hi \"there\")";

    root = parse_all(script);

    test(suite, root->branch->value->type == SL_TYPE_SYMBOL, "let is symbol");
    test(suite, string_cmp(root->branch->value->slot.string, str("let")) == 0, "let is the content of the symbol");
    test(suite, root->branch->next->value->type == SL_TYPE_KEY, ".hi is a key");
    test(suite, string_cmp(root->branch->next->value->slot.string, str("hi")) == 0, "key name is accurate");

    script = "(print \"the sum is: \" (+ 1 2) \" units\")";

    root = parse_all(script);

    cell = root->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "print is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the content of the symbol");

    cell = root->branch->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "second is string");
    test(suite, string_cmp(cell->value->slot.string, str("the sum is: ")) == 0, "the sum is string is the content of the string");
    
    /*
    printf("%s\n", script);
    print_cell(root->branch);
    print_cell(root->branch->next);
    print_cell(root->branch->next->branch);
    print_cell(root->branch->next->next);
    */

    cell = root->branch->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "+ is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("+")) == 0, "+ is the content of the symbol");

    cell = root->branch->next->branch->next;
    test(suite, cell->value->type == SL_TYPE_INT, "1 is an integer");
    test(suite, cell->value->slot.integer == 1, "1 is 1");

    cell = root->branch->next->branch->next->next;
    test(suite, cell->value->type == SL_TYPE_INT, "2 is an integer");
    test(suite, cell->value->slot.integer == 2, "2 is 2");

    cell = root->branch->next->branch->next->next;
    print_cell(cell);
    test(suite, cell->next == NULL, "next cell next is null");

    /*
    printf("root: ");
    print_cell(root);
    printf("root->branch: ");
    print_cell(root->branch);
    printf("root->branch->next: ");
    print_cell(root->branch->next);
    printf("\n");
    */

    cell = root->branch->next->next;
    print_cell(root->branch->next);
    test(suite, cell->value->type == SL_TYPE_STRING, "third section is string");
    test(suite, string_cmp(cell->value->slot.string, str(" units")) == 0, "units string is the content of the string");

    summerize(suite);

    /********************************* Swap for symbol test ********************/
    suite = new_suite("Swap for symbol test");
    struct closure *parent = new_closure(NULL);
    struct closure *child = new_closure(parent);

    struct value_obj *actual_value = new_int_value_obj(10);
    struct value_obj *symbol_value = new_symbol_value_obj(str("ten"));
    tree_add(parent->symbols, str("ten"), actual_value);

    struct value_obj *returned = swap_for_symbol(child, symbol_value);

    test(suite, returned->type == SL_TYPE_INT, "returned is an int");
    test(suite, returned->slot.integer == 10, "returned is the content of the symbol");


    summerize(suite);
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
