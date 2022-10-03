void test_head(){
    struct suite *suite = NULL;
    struct order_entry *order_entry = NULL;
    struct value_obj *result = NULL;

    suite = new_suite("Head tests");
    struct head *head = NULL;
    struct cell *cell = NULL;
    struct closure *closure = NULL;
    struct value_obj *value = NULL;
    struct operator_ifc *op;
    struct string *op_name;

    cell = new_cell(NULL);
    closure = new_closure(NULL);
    head = new_head();

    setup_new_head(head, cell, closure);
    test(suite, head->operator->type == DEFAULT, "No operator if cell has no value");

    value = new_value();
    value->type == SL_TYPE_STRING;
    cell->value = value;

    setup_new_head(head, cell, closure);
    test(suite, head->operator->type == DEFAULT, "Operator should not be set if value is not function");

    op = new_arithmetic_operator(ADD);
    op_name = str("test-op");

    closure_add_function(closure, op_name, op);

    cell->value = new_value();
    cell->value->type = SL_TYPE_SYMBOL;
    cell->value->slot.string = op_name;

    setup_new_head(head, cell, closure);
    test(suite, head->operator != NULL, "Operator should be set from closure");

    summerize(suite);
}
