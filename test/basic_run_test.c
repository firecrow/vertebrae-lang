void test_basic_run(){
    suite = new_suite("Run tests");

    script = "add < 1 3 5";

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    test(suite, state->value->type == SL_TYPE_INT, "head value is int");
    test(suite, state->value->slot.integer == 9, "arithmetic value is the content of the cells");

    summerize(suite);
}
