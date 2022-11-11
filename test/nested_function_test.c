void test_nested_function(){
    suite = new_suite("Nested function tests");

    global = new_closure(NULL);
    init_basic_library(global); 

    script = "(.hi \"there\" (let (save-head \"say, \" hi)))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");

    struct value_obj *key = new_symbol_value_obj(str("hi"));
    struct value_obj *hi_value = swap_for_symbol(state->data->slot.head->closure, key);

    test(suite, !string_cmp(hi_value->slot.string, str("there")), "symbol 'hi' should have value 'there'");

    summerize(suite);
}
