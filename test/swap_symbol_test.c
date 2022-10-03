void test_swap_symbol(){
    /********************************* Swap for symbol test ********************/
    suite = new_suite("Swap for symbol test");
    struct closure *parent = new_closure(NULL);
    struct closure *child = new_closure(parent);

    struct value_obj *actual_value = new_int_value_obj(10);
    struct value_obj *symbol_value = new_symbol_value_obj(str("ten"));
    tree_add(parent->symbols, str("ten"), actual_value);

    struct value_obj *returned = swap_for_symbol(child, symbol_value);

    test(suite, returned->type == SL_TYPE_INT, "returned is an int");
    test(suite, returned->slot.integer == 10, "returned is the content of the symbol");


    summerize(suite);
}
