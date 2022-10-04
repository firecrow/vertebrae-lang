void test_steps(){
    suite = new_suite("Step tests");
    struct crw_state *state = NULL;

    root = new_cell(NULL);
    first = new_cell(NULL);
    root->next = first;

    global = new_closure(NULL);
    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    test(suite, state->cell == root, "Cell are assinged");
    test(suite, state->head->closure->parent == global, "Global are assinged");

    head = new_head();

    summerize(suite);
}
