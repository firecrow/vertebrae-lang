void test_functions(){
    suite = new_suite("Function test");

    /*
    root = new_cell(NULL);
    first = new_cell(NULL);
    global = new_closure(NULL);

    root = new_cell(NULL);
    root->value = new_symbol_value_obj(str("let"));
    root->id = 1;

    second = new_cell(NULL);
    second->value = new_key_value_obj(str("show"));
    second->id = 2;

        fourth = new_cell(NULL);
        fourth->value = new_symbol_value_obj(str("print"));
        fourth->id = 4;

        fifth = new_cell(NULL);
        fifth->value = new_symbol_value_obj(str("message"));
        fifth->id = 5;

    third = new_cell(NULL);
    third->id = 3;

    sixth = new_cell(NULL);
    sixth->id = 6;

    seventh = new_cell(NULL);
    seventh->value = new_symbol_value_obj(str("show"));
    seventh->id = 7;

    eigth = new_cell(NULL);
    eigth->value = new_key_value_obj(str("message"));
    eigth->id = 8;

    ninth = new_cell(NULL);
    ninth->value = new_string_value_obj(str("hello, crow-lisp"));
    ninth->id = 8;

    root->next = first;
    first->next = second;
    second->next = third;
    third->branch = fourth;
    fourth->next = fifth;
    third->next = sixth;
    sixth->branch = seventh;
    seventh->next = eigth;
    eigth->next = ninth;

    state = crw_new_state_context();
    crw_setup_state_context(state, root, global);

    while(state->status != CRW_DONE){
       state->next(state); 
    }

    */
    summerize(suite);
}
