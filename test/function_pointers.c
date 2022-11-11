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

    print_value(state->data->slot.value);
    printf("\n");
    test(suite, state->data != NULL, "test basic function runs: save-head is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_INT, "mock value is int");
    test(suite, state->data->slot.value->slot.integer == 3, "mock value is sum of numbers");

    /* test function set variable */
    script = "(.func '(save-value name)\n  (func .name \"sam\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    print_value(state->data->slot.value);
    printf("\n");
    test(suite, state->data != NULL, "test basic function runs: save-value is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_STRING, "mock value is string");
    test(suite, !string_cmp(state->data->slot.value->slot.string, str("sam")), "mock value is key set on call");

    /* test nested heads */
    script = "(((save-value \"alpha\")))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    printf("running\n");
    run_root(state, root);

    test(suite, state->data != NULL, "test basic function runs: save-value is not null");
    test(suite, state->data->type == SL_TYPE_VALUE, "mock value head set");
    test(suite, state->data->slot.value->type == SL_TYPE_STRING, "mock value is string");
    test(suite, !string_cmp(state->data->slot.value->slot.string, str("alpha")), "mock value is nested parans avlue");

    /* test nested heads side by side */

    /*
    script = "( .default \"hello\" .level 10 .nested-func '( if (false) '(print \"the right answer is \" (+ 1)) (true) '(print \"the wrong answer is \" (+ 3))) .set-it '(^default \"hi\") .prop-func '((print .tree) (print default \" \" name)) .list-func '(print default \" \" value \", how are you?\") (print .data) (set-it) (prop-func .name \"sally\") (list-func \"bear\" \"dummy\" \"nail\" \"scratch\") (.func (nested-func)) (func))";*/

    script = "(.default \"hello \" .prop-func '((print default \" \" name)) (prop-func .name \"sally\"))";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    summerize(suite);
}
