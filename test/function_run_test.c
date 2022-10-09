void test_run_functions(){

    global = new_closure(NULL);
    init_basic_library(global); 

    state = crw_new_state_context();

    suite = new_suite("Function run tests");

    script = "(\n    print (save-head (+ 1 2 3)))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_HEAD, "mock head set");
    test(suite, state->data->slot.head->value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.head->value->slot.integer == 6, "mock value is sum of numbers");

    script = "(save-cell 10)";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    print_cell(state->data->slot.cell);
    test(suite, state->data->type == SL_TYPE_CELL, "mock-cell set");
    test(suite, state->data->slot.cell->value->type == SL_TYPE_INT, "mock-cell value is int");
    test(suite, state->data->slot.cell->value->slot.integer == 10, "mock-cell value is sum of numbers");

    script = "(save-value (+ 1))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->data->type == SL_TYPE_VALUE, "mock value set");
    test(suite, state->data->slot.value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.value->slot.integer == 1, "mock value is the number");

    script = "(\n    let .hi \"there\"\n    .func '(\n        print hi))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    printf("no print runs");
    test(suite, state->context == NULL, "mock never runs");


    script = "(\n    let .hi \"there\"\n    .func '(\n        print hi)\n    (func))";
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

    script = "(\n    let .hi \"there\"\n    .func '(\n        mock (print \"hello, \" value))\n    (func \"one\" \"two\" \"three\"))";
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
