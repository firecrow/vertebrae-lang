void test_namespaces(){
    suite = new_suite("Namespace tests");

    global = new_closure(NULL);
    init_basic_library(global); 

    script = "(save-head .hi \"there\")";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");

    struct value_obj *hi_value = tree_get(state->data->slot.head->closure->symbols, str("hi"));
    test(suite, !string_cmp(hi_value->slot.string, str("there")), "symbol 'hi' should have value 'there'");

    summerize(suite);
}
