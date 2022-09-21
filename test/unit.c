#include "../src/gekkota.h"

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
    test(suite, head->operator->type == DEFAULT, "No operator if cell has no value");

    value = new_value();
    value->type == SL_TYPE_STRING;
    cell->value = value;

    setup_new_head(head, cell, closure);
    test(suite, head->operator->type == DEFAULT, "Operator should not be set if value is not function");

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


    root = new_cell();
    first = new_cell();
    root->next = first;

    global = new_closure(NULL);
    state = crw_new_state_context(root, global);

    test(suite, state->cell == root, "Cell are assinged");
    test(suite, state->head->closure->parent == global, "Global are assinged");
    test(suite, state->stack != NULL, "Stack are assinged");

    head = new_head();

    /* test a few next steps */
    suite = new_suite("Basic step tests");

    root = new_cell();
    first = new_cell();
    second = new_cell();
    third = new_cell();
    fourth = new_cell();

    root->next = first;
    first->next = second;
    second->next = third;
    third->next = fourth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context(root, stack);

    state->next(state);
    test(suite, state->cell == first, "First cell should be current cell after step");

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

    first = new_cell();
    first->value = new_string_value_obj(str("first"));

    second = new_cell();
    second->value = new_string_value_obj(str("second"));

    third = new_cell();
    third->value = new_string_value_obj(str("third"));

    fourth = new_cell();
    fourth->value = new_string_value_obj(str("fourth"));
    
    fifth = new_cell();
    fifth->value = new_string_value_obj(str("fifth"));

    sixth = new_cell();
    sixth->value = new_string_value_obj(str("sixth"));

    root->next = first;
    first->next = second;
    second->next = third;
    second->branch = fourth;
    fourth->next = fifth;
    third->next = sixth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context(root, stack);

    state->next(state);
    test(suite, state->cell == first, "First cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == second, "Second cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == fifth, "Fifth step is the next after the branch cell(fourth)");

    state->next(state);
    test(suite, state->cell == third, "Fourth cell should be on main path again (Third)");

    state->next(state);
    test(suite, state->cell == sixth, "Sixth cell should be on main path as well (Fifth)");

    summerize(suite);

    /********************************* test arithmetic ********************/
    suite = new_suite("Arithmetic tests");

    root = new_cell();
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell();
    branch = new_cell();
    branch->value = new_symbol_value_obj(str("+"));

    second = new_cell();
    second->value = new_int_value_obj(1);

    third = new_cell();
    third->value = new_int_value_obj(3);

    fourth = new_cell();
    fourth->value = new_int_value_obj(-2);

    root->branch = branch;
    branch->next = second;
    second->next = third;
    third->next = fourth;

    state = crw_new_state_context(root, global);

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    test(suite, state->head->value->slot.integer == 2, "Arithemtic comes up with proper value");

    summerize(suite);

    /********************************* Variable Assignment Tests ********************/
    suite = new_suite("Variable assignment tests");

    root = new_cell();
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell();
    root->value = new_symbol_value_obj(str("let"));

    second = new_cell();
    second->value = new_key_value_obj(str("one"));

    third = new_cell();
    third->value = new_int_value_obj(1);

    fourth = new_cell();

    fifth = new_cell();
    fifth->value = new_symbol_value_obj(str("+"));

    sixth = new_cell();
    sixth->value = new_int_value_obj(2);

    seventh = new_cell();
    seventh->value = new_symbol_value_obj(str("one"));

    root->next = second;
    second->next = third;
    third->next = fourth;
    fourth->branch = fifth;
    fifth->next = sixth;
    sixth->next = seventh;

    state = crw_new_state_context(root, global);

    while(state->status != CRW_DONE){
       state->next(state);
    }

    test(suite, tree_get(state->head->closure->symbols, str("one"))->slot.integer = 1,"Variable one is set with value 1");
    test(suite, state->head->value->slot.integer == 3, "Final value reflects variable value");

    summerize(suite);
    
    /********************************* Function pointer test ********************/
    suite = new_suite("Function test");

    root = new_cell();
    first = new_cell();
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell();
    root->value = new_symbol_value_obj(str("let"));
    root->id = 1;

    /* this is the symbol that will store the function branch */
    second = new_cell();
    second->value = new_key_value_obj(str("show"));
    second->id = 2;

        fourth = new_cell();
        fourth->value = new_symbol_value_obj(str("print"));
        fourth->id = 4;

        fifth = new_cell();
        fifth->value = new_symbol_value_obj(str("message"));
        fifth->id = 5;

    /* atach the new function */
    third = new_cell();
    third->id = 3;

    sixth = new_cell();
    sixth->id = 6;

    seventh = new_cell();
    seventh->value = new_symbol_value_obj(str("show"));
    seventh->id = 7;

    eigth = new_cell();
    eigth->value = new_key_value_obj(str("message"));
    eigth->id = 8;

    ninth = new_cell();
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

    state = crw_new_state_context(root, global);

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    summerize(suite);

    /********************************* Parse test ********************/
    suite = new_suite("Parse tests");
    char *script = "(+ 127)";

    root = parse_all(script);
    
    print_cell(root);
    printf("\n");
    print_cell(root->branch);
    printf("\n");
    print_cell(root->branch->next);
    printf("\n");

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
    test(suite, string_cmp(cell->value->slot.string, str("the sum is: ")) == 0, "string is the content of the string");
    
    cell = root->branch->next->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "+ is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("+")) == 0, "+ is the content of the symbol");

    cell = root->branch->next->next->branch->next;
    test(suite, cell->value->type == SL_TYPE_INT, "1 is an integer");
    test(suite, cell->value->slot.integer == 1, "1 is 1");

    cell = root->branch->next->next->branch->next->next;
    test(suite, cell->value->type == SL_TYPE_INT, "2 is an integer");
    test(suite, cell->value->slot.integer == 2, "2 is 2");

    cell = root->branch->next->next->branch->next->next->next;
    test(suite, cell->value == NULL, "next cell value is null");
    test(suite, cell->next == NULL, "next cell next is null");

    cell = root->branch->next->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "third section is string");
    test(suite, string_cmp(cell->value->slot.string, str(" units")) == 0, "string is the content of the string");

    summerize(suite);

    return 0;
}
