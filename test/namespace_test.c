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

    print_head(state->data->slot.head);
    printf("\n");
    print_tree(state->data->slot.head->closure->symbols);
    printf("\n");

}
