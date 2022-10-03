void test_parse(){
    suite = new_suite("Parse tests");
    char *script = "(+ 127)";

    root = parse_all(script);

    /*
    printf("root: ");
    print_cell(root);
    printf("root->branch: ");
    print_cell(root->branch);
    printf("root->next: ");
    print_cell(root->next);
    printf("\n");
    */

    test(suite, root->branch->value->type == SL_TYPE_SYMBOL, "+ is symbol");
    test(suite, string_cmp(root->branch->value->slot.string, str("+")) == 0, "+ is the content of the symbol");
    test(suite, root->branch->next->value->type == SL_TYPE_INT, "1 is an int");
    test(suite, root->branch->next->value->slot.integer == 127, "1 is 127");

    script = "(let .hi \"there\")";

    root = parse_all(script);

    test(suite, root->branch->value->type == SL_TYPE_SYMBOL, "let is symbol");
    test(suite, string_cmp(root->branch->value->slot.string, str("let")) == 0, "let is the content of the symbol");
    test(suite, root->branch->next->value->type == SL_TYPE_KEY, ".hi is a key");
    test(suite, string_cmp(root->branch->next->value->slot.string, str("hi")) == 0, "key name is accurate");

    script = "(print \"the sum is: \" (+ 1 2) \" units\")";

    root = parse_all(script);

    cell = root->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "print is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the content of the symbol");

    cell = root->branch->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "second is string");
    test(suite, string_cmp(cell->value->slot.string, str("the sum is: ")) == 0, "the sum is string is the content of the string");
    
    /*
    printf("%s\n", script);
    print_cell(root->branch);
    print_cell(root->branch->next);
    print_cell(root->branch->next->branch);
    print_cell(root->branch->next->next);
    */

    cell = root->branch->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "+ is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("+")) == 0, "+ is the content of the symbol");

    cell = root->branch->next->branch->next;
    test(suite, cell->value->type == SL_TYPE_INT, "1 is an integer");
    test(suite, cell->value->slot.integer == 1, "1 is 1");

    cell = root->branch->next->branch->next->next;
    test(suite, cell->value->type == SL_TYPE_INT, "2 is an integer");
    test(suite, cell->value->slot.integer == 2, "2 is 2");

    cell = root->branch->next->branch->next->next;
    print_cell(cell);
    test(suite, cell->next == NULL, "next cell next is null");

    /*
    printf("root: ");
    print_cell(root);
    printf("root->branch: ");
    print_cell(root->branch);
    printf("root->branch->next: ");
    print_cell(root->branch->next);
    printf("\n");
    */

    cell = root->branch->next->next;
    print_cell(root->branch->next);
    test(suite, cell->value->type == SL_TYPE_STRING, "third section is string");
    test(suite, string_cmp(cell->value->slot.string, str(" units")) == 0, "units string is the content of the string");

    summerize(suite);
}
