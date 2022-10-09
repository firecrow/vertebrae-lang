void test_pop_stack(){
    /********************************* test pop_stack ********************/
    suite = new_suite("Pop stack branch tests");

    root = new_cell(NULL);
    root->value = new_string_value_obj(str("root"));

    first = new_cell(NULL);
    first->value = new_string_value_obj(str("first"));

    /* branch cell */
    second = new_cell(NULL);

    third = new_cell(NULL);
    third->value = new_string_value_obj(str("third"));

    fourth = new_cell(NULL);
    fourth->value = new_string_value_obj(str("fourth"));
    
    fifth = new_cell(NULL);
    fifth->value = new_string_value_obj(str("fifth"));

    sixth = new_cell(NULL);
    sixth->value = new_string_value_obj(str("sixth"));

    /* 
     * ("first" ("third" fourth") "fifth" "sixth")
     */
    root->next = first;
    first->next = second;
    second->next = fifth;
        second->branch = third;
        third->next = fourth;
    fifth->next = sixth;

    global = new_closure(NULL);
    stack = new_stack_item(NULL, root, new_head());
    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    state->next(state);
    test(suite, state->cell == first, "First cell should be current cell after step");

    state->next(state);
    test(suite, state->cell == second, "Second step is the next");

    test(suite, state->cell != third, "Third step skipped because its the head");

    state->next(state);
    test(suite, state->cell == fourth, "Fourth step is the next after the branch cell(fourth)");

    state->next(state);
    test(suite, state->cell == fifth, "Fifth cell should be on main path again (Third)");

    state->next(state);
    test(suite, state->cell == sixth, "Sixth cell should be on main path as well (Fifth)");

    summerize(suite);
}
