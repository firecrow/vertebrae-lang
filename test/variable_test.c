void test_variables(){
    suite = new_suite("Variable assignment tests");

    root = new_cell(NULL);
    global = new_closure(NULL);

    init_basic_library(global); 

    root = new_cell(NULL);
    root->value = new_symbol_value_obj(str("let"));

    second = new_cell(NULL);
    second->value = new_key_value_obj(str("one"));

    third = new_cell(NULL);
    third->value = new_int_value_obj(1);

    fourth = new_cell(NULL);

    fifth = new_cell(NULL);
    fifth->value = new_symbol_value_obj(str("+"));

    sixth = new_cell(NULL);
    sixth->value = new_int_value_obj(2);

    seventh = new_cell(NULL);
    seventh->value = new_symbol_value_obj(str("one"));

    root->next = second;
    second->next = third;
    third->next = fourth;
    fourth->branch = fifth;
    fifth->next = sixth;
    sixth->next = seventh;
    
    /* 
     * (.one 1 (+ 2 one))
     */

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    while(state->status != CRW_DONE){
       state->next(state);
    }

    test(suite, tree_get(state->head->closure->symbols, str("one"))->slot.integer = 1,"Variable one is set with value 1");
    test(suite, state->head->value->slot.integer == 3, "Final value reflects variable value");

    /* set lext test */
    global = new_closure(NULL);
    init_basic_library(global); 
    char script[] = "(.greeting \"hello\" ^greeting \"hi\" (save-value greeting))";

    printf("%s\n", script);

    root = parse_all(script);

    state = crw_new_state_context();
    run_root(state, root);

    test(suite, !string_cmp(state->data->slot.value->slot.string, str("hi")), "Variable was updated");

    summerize(suite);
}
