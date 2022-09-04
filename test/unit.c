#include "../src/external.h"

#include "../src/ssimple.h"
#include "../src/core/core.h"
#include "../src/types/types.h"
#include "../src/run/run.h"
#include "../src/operators/operator.h"
#include "../src/library/library.h"

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
    printf("SUITE ************ %s ************\n", name);
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
    printf(" *%s* %d PASSED, %d FAILED %s%s\n", suite->name, suite->passed, suite->failed, suite->name, NUETRAL);
}

struct suite *suite = NULL;
struct order_entry *order_entry = NULL;
struct value_obj *result = NULL;
int main(){

    /***************** TEST SUITE ************
    suite = new_suite("Test Suite");
    test(suite, 1, "Testing success");
    test(suite, 0, "Testing failure");
    summerize(suite);
    */

    /***************** TREE SUITE *************/
    suite = new_suite("Tree");
    struct tree *tree = new_tree();

    struct value_obj *alpha = new_string_value_obj(str("apples"));
    struct string *alpha_key = str("alpha");

    struct value_obj *bravo = new_string_value_obj(str("berries"));
    struct string *bravo_key = str("bravo");

    struct value_obj *charlie = new_string_value_obj(str("charlie"));
    struct string *charlie_key = str("carrots");

    tree_add(tree, alpha_key, alpha);
    test(suite, tree->count == 1, "Tree count is 1 for first record");

    result = tree_get(tree, alpha_key);
    test(suite, !strncmp(result->slot.string->content, alpha->slot.string->content, 4096), "Retrieved result matches"); 
    
    test(suite, tree->order->entry->content == alpha , "Order starts with first record"); 

    tree_add(tree, bravo_key, bravo);
    test(suite, tree->count == 2, "Tree count is 2 for second record");

    order_entry = tree->order;
    test(suite, order_entry->next->entry->content == bravo, "Second order points to second record"); 

    test(suite, order_entry->next->previous == order_entry, "Order entry is double linked"); 

    result = tree_get(tree, bravo_key);
    test(suite, !strncmp(result->slot.string->content, bravo->slot.string->content, 4096), "Retrieved result matches"); 

    tree_add(tree, charlie_key, charlie);
    test(suite, tree->count == 3, "Tree count is 3 for third record");

    result = tree_get(tree, charlie_key);
    test(suite, !strncmp(result->slot.string->content, charlie->slot.string->content, 4096), "Retrieved result matches"); 

    summerize(suite);

    /***************** TREE SUITE *************/
    suite = new_suite("Head tests");
    struct head *head = NULL;
    struct cell *cell = NULL;
    struct closure *closure = NULL;
    struct value_obj *value = NULL;
    struct operator_ifc *op;
    struct string *op_name;

    cell = new_cell();
    closure = new_closure(NULL);
    head = new_head();

    setup_new_head(head, cell, closure);
    test(suite, head->operator == NULL, "No operator if cell has no value");

    value = new_value();
    value->type == SL_TYPE_STRING;
    cell->value = value;

    setup_new_head(head, cell, closure);
    test(suite, head->operator == NULL, "Operator should not be set if value is not function");

    op = new_arithmetic_operator(ADD);
    op_name = str("test-op");

    closure_add_function(closure, op_name, op);

    cell->value = new_value();
    cell->value->type = SL_TYPE_SYMBOL;
    cell->value->slot.string = op_name;

    setup_new_head(head, cell, closure);
    test(suite, head->operator != NULL, "Operator should be set from closure");

    summerize(suite);

    /***************** STEP TESTS *************/
    struct crw_state *state = NULL;

    struct cell *root = NULL;
    struct closure *global = NULL;
    struct stack_item *stack = NULL;
    struct cell *second = NULL;
    struct cell *third = NULL;
    struct cell *fourth = NULL;
    struct cell *fifth = NULL;

    head = new_head();
    root = new_cell();
    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context(root, global, stack);

    test(suite, state->cell == root, "Cell are assinged");
    test(suite, state->closure == global, "Global are assinged");
    test(suite, state->stack == stack, "Stack are assinged");

    head = new_head();

    /* test a few next steps */
    suite = new_suite("Basic step tests");

    root = new_cell();
    second = new_cell();
    third = new_cell();
    fourth = new_cell();
    root->next = second;
    second->next = third;
    third->next = fourth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context(root, global, stack);

    state->next(state);
    test(suite, state->cell == second, "Second cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == third, "Third cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == fourth, "Fourth cell should be current cell after step");
    
    summerize(suite);

    /********************************* test pop_stack ********************/
    suite = new_suite("Pop stack branch tests");

    root = new_cell();
    root->value = new_string_value_obj(str("root"));

    second = new_cell();
    second->value = new_string_value_obj(str("second"));

    third = new_cell();
    third->value = new_string_value_obj(str("third"));

    fourth = new_cell();
    fourth->value = new_string_value_obj(str("fourth"));
    
    fifth = new_cell();
    fifth->value = new_string_value_obj(str("fifth"));

    root->next = second;
    second->next = third;
    second->branch = fourth;
    third->next = fifth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context(root, global, stack);

    state->next(state);
    test(suite, state->cell == second, "Second cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == fourth, "Third step is branch cell(fourth)");

    state->next(state);
    test(suite, state->cell == third, "Fourth cell should be on main path again (Third)");

    state->next(state);
    test(suite, state->cell == fifth, "Fifth cell should be on main path as well (Fifth)");

    summerize(suite);

    /********************************* test arithmetic ********************/
    suite = new_suite("Arithmetic tests");

    root = new_cell();
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell();
    root->value = new_symbol_value_obj(str("+"));

    second = new_cell();
    second->value = new_int_value_obj(1);

    third = new_cell();
    third->value = new_int_value_obj(3);

    fourth = new_cell();
    fourth->value = new_int_value_obj(-2);

    root->next = second;
    second->next = third;
    third->next = fourth;

    stack = new_stack_item(NULL, NULL, new_head());
    state = crw_new_state_context(root, global, stack);

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    test(suite, state->head->value->slot.integer == 2, "Arithemtic comes up with proper value");

    return 0;
}
