void test_if_else(){
    suite = new_suite("If/Else tests");
    struct value_obj *value;

    global = new_closure(NULL);
    init_basic_library(global); 

    /* basic test of truthy runs next */
    script = "(if true (save-cell \"yes\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of truthy runs next");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("yes\n")), "value yes should be assined indicating it ran");

    /* basic test of truthy does not run next */
    script = "(if false (save-cell \"yes\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "basic test of truthy does not run next");

    /* basic test of else */

    script = "(if false (save-cell \"yes\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'no' cell is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no\n")), "basic test of else");

    script = "(if false (save-cell \"yes\n\") (\"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "mock 'yes' cell is not set");

    /* basic test of else if else */

    script = "(if false (save-cell \"yes\n\") true (save-cell \"again\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("again\n")), "basic test of else if else");

    /* basic test of else if else with no second branch */

    script = "(if false (save-cell \"yes\n\") false false true (save-cell \"again\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else with no second branch");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("again\n")), "basic test of else if else with no second branch");

    /* basic test of else if else with two false */

    script = "(if false (save-cell \"yes\n\") false (save-cell \"again\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else with two false");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no\n")), "basic test of else if else with two false");

    /* basic test of else if else with two false and a true*/

    script = "(if false (save-cell \"yes\n\") false (save-cell \"again\n\") true (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'again' cell is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no\n")), "basic test of else if else with two false and a true");


    summerize(suite);
}