void test_basic(){
    /*************************** test a few next steps ***************/
    suite = new_suite("Basic step tests");

    root = new_cell(NULL);
    root->value = new_string_value_obj(str("root"));
    first = new_cell(NULL);
    first->value = new_string_value_obj(str("first"));
    second = new_cell(NULL);
    second->value = new_string_value_obj(str("second"));
    third = new_cell(NULL);
    third->value = new_string_value_obj(str("third"));
    fourth = new_cell(NULL);
    fourth->value = new_string_value_obj(str("fourth"));

    root->next = first;
    first->next = second;
    second->next = third;
    third->next = fourth;

    stack = new_stack_item(NULL, root, head);
    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    state->next(state);
    test(suite, state->cell == first, "First cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == second, "Second cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == third, "Third cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == fourth, "Fourth cell should be current cell after step");
    
    summerize(suite);
}
