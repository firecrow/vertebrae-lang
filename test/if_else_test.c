void test_if_else(){
    suite = new_suite("If/Else tests");
    struct value_obj *value;

    global = new_closure(NULL);
    init_basic_library(global); 

    script = "if << t < true, save-cell < \"yes\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of truthy runs next");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("yes")), "value yes should be assined indicating it ran");

    script = "if << t < false, save-cell < \"yes\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "basic test of truthy does not run next");


    script = "if << eq < 1 2, save-cell < \"yes\", eq < 1 1, save-cell < \"no\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'no' cell is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no")), "basic test of else");

    script = "if << t < false, save-cell < \"yes\", print < \"no\".";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "mock 'yes' cell is not set");


    script = "if <<\n    t < false, save-cell < \"yes\\n\",\n    t < true, save-cell < \"again\",\n    save-cell < \"no\"";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else");

    value = state->data->slot.cell->value;
    print_value(value);
    test(suite, !string_cmp(value->slot.string, str("again")), "basic test of else if else II");


    script = "if <<\n    t < false, save-cell < \"yes\",\n    t < false, save-cell < false,\n    t < true, save-cell < \"again\",\n    save-cell < \"no\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else with no second branch");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("again")), "basic test of else if else with no second branch II");


    script = "if <<\n    t < false, save-cell < \"yes\",\n    t < false, save-cell < \"again\",\n    save-cell < \"no\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "basic test of else if else with two false");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no")), "basic test of else if else with two false II");


    script = "if <<\n    t < false, save-cell < \"yes\",\n    t < false, save-cell < \"again\",\n    t < true, save-cell < \"no\",";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_CELL, "mock 'again' cell is set");

    value = state->data->slot.cell->value;
    test(suite, !string_cmp(value->slot.string, str("no")), "basic test of else if else with two false and a true III");

    summerize(suite);
}
