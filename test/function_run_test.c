void test_run_functions(){

    global = new_closure(NULL);
    init_basic_library(global); 
    state = crw_new_state_context();

    suite = new_suite("Function run tests");
    print_tree(global->symbols);

    script = "(\n    print (save-head (+ 1 2 3)))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    printf("data type: %d", state->data->type);
    print_head(state->data->slot.head);

    test(suite, state->context == NULL, "mock never runs");

    script = "(\n    .hi \"there\"\n    .func '(\n        print hi))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    printf("no print runs");
    test(suite, state->context == NULL, "mock never runs");


    script = "(\n    .hi \"there\"\n    .func '(\n        print hi)\n    (func))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    /*
    if(state->context){
        value = swap_for_symbol(state->context->closure, new_symbol_value_obj(str("hi")));
    }else{
        value = NULL;
    }
    print_head(state->context);

    test(suite, value && value->type == SL_TYPE_STRING, "returned is an string");
    test(suite, value && string_cmp(value->slot.string, str("there")) == 0, "after function call mock has 'there' value");
    */

    script = "(\n    .hi \"there\"\n    .func '(\n        mock (print \"hello, \" value))\n    (func \"one\" \"two\" \"three\"))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    if(state->context){
        value = swap_for_symbol(state->context->closure, new_symbol_value_obj(str("hi")));
    }else{
        value = NULL;
    }

    /*
    test(suite, value && value->type == SL_TYPE_STRING, "returned is an string II");
    test(suite, value && string_cmp(value->slot.string, str("there")) == 0, "after function call mock has 'there' value II");
    */


    summerize(suite);
}
