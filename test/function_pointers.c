void test_function_pointers(){

    global = new_closure(NULL);
    init_basic_library(global); 

    state = crw_new_state_context();

    suite = new_suite("Function pointer run tests");

    /* test basic function does not run */
    script = "(\n  .func '(save-head (+ 1 2 3)))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data == NULL, "test basic function does not run: save-head is null");

    /* test basic function runs */
    script = "(\n  .func '(save-head (+ 1 2 3)) (func))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data != NULL, "test basic function runs: save-head is not null");
    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");
    test(suite, state->data->slot.head->value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.head->value->slot.integer == 6, "mock value is sum of numbers");

    /* test basic function runs a count of times */
    script = "(\n  .func '(\n      save-count true)\n  (func 1 2 3))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    printf("\x1b[33m");
    print_value(state->data->slot.value);
    printf("\n\x1b[0m");
    test(suite, state->data != NULL, "test basic function runs: save-head is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.value->slot.integer == 1, "mock value is sum of numbers");

    summerize(suite);
}
