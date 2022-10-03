void test_basic_run(){
    suite = new_suite("Run tests");

    script = "(+ 1 3 5)";

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->head->value->type == SL_TYPE_INT, "head value is int");
    test(suite, state->head->value->slot.integer == 9, "arithmetic valu is the cntent of the cells");

    script = "(.hi \"there\" (mock))";
    printf("%s\n", script);

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    value = state->head->value;
    print_value(value);

    test(suite, value->type == SL_TYPE_STRING, "returned is an int");
    test(suite, string_cmp(value->slot.string, str("there")) == 0, "hi key has 'there' value");

    summerize(suite);
}
