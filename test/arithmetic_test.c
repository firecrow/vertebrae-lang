void test_arithmetic(){
    suite = new_suite("Arithmetic tests");

    global = new_closure(NULL);
    init_basic_library(global); 

    root = new_cell(NULL);
    root->value = new_result_value_obj(NIL);

    branch = new_cell(NULL);
    branch->value = new_symbol_value_obj(str("+"));

    second = new_cell(NULL);
    second->value = new_int_value_obj(1);

    third = new_cell(NULL);
    third->value = new_int_value_obj(3);

    fourth = new_cell(NULL);
    fourth->value = new_int_value_obj(-2);

    root->branch = branch;

    branch->next = second;
    second->next = third;
    third->next = fourth;

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    /*
     * (+ 1 3 -2) 
     */

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    test(suite, state->head->value->slot.integer == 2, "Arithemtic comes up with proper value");

    summerize(suite);
}
