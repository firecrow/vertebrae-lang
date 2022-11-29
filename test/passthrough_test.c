void test_passthrough(){
    suite = new_suite("Passthrough tests");

    global = new_closure(NULL);
    init_basic_library(global); 

    script = "save-head < + < 7";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);


    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");

    struct value_obj *value = state->data->slot.head->value;
    test(suite, value->type == SL_TYPE_INT, "head has value from nested, type is int");
    test(suite, value->slot.integer == 7, "head has value from nested, in is 7");

    summerize(suite);
}
