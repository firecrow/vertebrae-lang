void test_basic_run(){
    suite = new_suite("Run tests");

    script = "(+ 1 3 5)";

    root = parse_all(script);
    state = crw_new_state_context();
    run_root(state, root);

    print_head(state->head);
    test(suite, state->head->value->type == SL_TYPE_INT, "head value is int");
    test(suite, state->head->value->slot.integer == 9, "arithmetic value is the content of the cells");

    summerize(suite);
}
