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

    test(suite, state->data->type == SL_TYPE_CELL, "mock cell 'yes' is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("yes\n")), "value yes should be assined indicating it ran");

    /* basic test of truthy does not run next */
    script = "(if false (save-cell \"yes\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "mock cell did not run");

    /* basic test of else */

    script = "(if false (save-cell \"yes\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'no' cell is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no\n")), "value 'no' should be assined indicating the else it ran");

    script = "(if false (save-cell \"yes\n\") (let \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "mock 'yes' cell is not set");

    /* basic test of else if */

    script = "(if false (save-cell \"yes\n\") true (save-cell \"again\n\") (save-cell \"no\n\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'again' cell is set");

    value = state->data->slot.cell->value;
    print_value(value);
    test(suite, !string_cmp(value->slot.string, str("again\n")), "value 'again' should be assined indicating it ran");

    summerize(suite);
}
