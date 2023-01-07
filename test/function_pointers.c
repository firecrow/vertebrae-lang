void test_function_pointers(){

    global = new_closure(NULL);
    init_basic_library(global); 

    state = crw_new_state_context();

    suite = new_suite("Function pointer run tests");

    /* test basic function does not run */
    script = "func -> save-head <\n    add <\n        1 2 3,.";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "test basic function does not run: save-head is null");

    /* test basic function runs */
    script = "func -> save-head < add < 1 2 3,.\n func < _";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data != NULL, "test basic function runs: save-head is not null");
    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");
    test(suite, state->data->slot.head->value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.head->value->slot.integer == 6, "mock value is sum of numbers");

    /* test basic function runs a count of times */
    script = "func -> save-count < true.\nfunc < 1 2 3";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    print_value(state->data->slot.value);
    printf("\n");
    test(suite, state->data != NULL, "test basic function runs: save-head is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.value->slot.integer == 3, "mock value is sum of numbers");

    /* test function set variable */
    script = ":name < \"sam\",\nfunc -> save-value < value.\nfunc < name";
    printf("%s\n", script);

    root = parse_all(script);
    print_branches(root, 0);

    state = crw_new_state_context();
    run_root(state, root);

    print_value(state->data->slot.value);

    test(suite, state->data != NULL, "test basic function runs: save-value is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_STRING, "mock value is string");
    test(suite, !string_cmp(state->data->slot.value->slot.string, str("sam")), "mock value is key set on call");

    summerize(suite);
}
