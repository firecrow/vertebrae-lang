void test_function_value_variable(){
    suite = new_suite("Function value variable tests");
    global = new_closure(NULL);
    init_basic_library(global); 
    
    script = "(.func '(save-value value) (func 13))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_VALUE, "type is value");
    test(suite, state->data->slot.value->slot.integer == 13, "value is value");

    summerize(suite);
}
