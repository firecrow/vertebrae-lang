void test_siblings(){

    global = new_closure(NULL);
    init_basic_library(global); 

    state = crw_new_state_context();

    suite = new_suite("Sibling tests");

    script = "print < \"hi\", print < \"there\"";
    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    summerize(suite);
}
