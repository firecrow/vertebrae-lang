void test_parse(){
    struct cell *start;
    struct cell *func;

    suite = new_suite("Parse tests");
    char *script = "add < 127,";

    root = parse_all(script);
    start = root->next->next;

    test(suite, start->branch->value->type == SL_TYPE_SYMBOL, "add is symbol");
    test(suite, string_cmp(start->branch->value->slot.string, str("add")) == 0, "add is the content of the symbol");
    test(suite, start->branch->next->value->type == SL_TYPE_INT, "1 is an int");
    test(suite, start->branch->next->value->slot.integer == 127, "1 is 127");

    script = ":hi < \"there\",";

    root = parse_all(script);

    start = root->next->next;
    test(suite, start->branch->value->type == SL_TYPE_SYMBOL, "hi is symbol");
    test(suite, string_cmp(start->branch->value->slot.string, str("hi")) == 0, "hi is the symbol name");
    test(suite, string_cmp(start->branch->next->value->slot.string, str("there")) == 0, "string is the approprate value");

    script = "print < \"the sum is: \" add < 1 2, \" units\",";

    root = parse_all(script);
    printf("%s\n", script);

    cell = root->next->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "print is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the content of the symbol");

    print_cell(root->next->next->branch->next);
    cell = root->next->next->branch->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "second is string");
    test(suite, string_cmp(cell->value->slot.string, str("the sum is: ")) == 0, "the sum is string is the content of the string");

    cell = root->next->next->branch->next->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "add is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("add")) == 0, "add is the content of the symbol");

    cell = root->next->next->branch->next->next->branch->next;
    test(suite, cell->value->type == SL_TYPE_INT, "1 is an integer");
    test(suite, cell->value->slot.integer == 1, "1 is 1");

    cell = root->next->next->branch->next->next->branch->next->next;
    test(suite, cell->value->type == SL_TYPE_INT, "2 is an integer");
    test(suite, cell->value->slot.integer == 2, "2 is 2");

    cell = root->next->next->branch->next->next->branch->next->next;
    test(suite, cell->next == NULL, "next cell next is null");

    cell = root->next->next->branch->next->next->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "third section is string");
    test(suite, string_cmp(cell->value->slot.string, str(" units")) == 0, "units string is the content of the string");

    script = "print < add < 1 2 3,,";
    printf("%s\n", script);

    root = parse_all(script);

    start = root->next->next;

    func = start->branch->next->branch;

    test(suite, string_cmp(func->value->slot.string, str("add")) == 0 , "func add is the branch");
    test(suite, func->next->value->slot.integer == 1, "next is the first value");

    script = "print << add < 1 2 3,.";
    printf("%s\n", script);

    root = parse_all(script);

    start = root->next->next;

    func = start->branch->next->branch->next->branch;

    test(suite, string_cmp(func->value->slot.string, str("add")) == 0 , "func add is the branch");
    test(suite, func->next->value->slot.integer == 1, "next is the first value");
    
    /* parse a function pointer */
    script = "func ->\n    print < \"hi\", print < \"there\".\nfunc < _";
    printf("%s\n", script);

    root = parse_all(script);

    start = root->next->next;

    test(suite, start->value->type == SL_TYPE_SYMBOL, "starts with the function symbol");
    test(suite, string_cmp(start->value->slot.string, str("func")) == 0, "first cell is func name");
    test(suite, start->value->accent = GKA_PARSE_DEF, "first cell has set accent");

    func = start->next->value->slot.cell;

    struct cell *part1 = func->branch;
    test(suite, string_cmp(part1->value->slot.string, str("print")) == 0, "part1 starts with print");
    test(suite, string_cmp(part1->next->value->slot.string, str("hi")) == 0, "part1 first arg is hi");
    printf("\n");
    struct cell *part2 = func->next->branch;
    test(suite, string_cmp(part2->value->slot.string, str("print")) == 0, "part2 starts with print");
    test(suite, string_cmp(part2->next->value->slot.string, str("there")) == 0, "part2 first arg is hello");

    printf("\n");
    struct cell *call = start->next->next->branch;
    test(suite, string_cmp(call->value->slot.string, str("func")) == 0, "call starts with print");
    test(suite, string_cmp(call->next->value->slot.string, str("_")) == 0, "call first arg is hello");

    summerize(suite);
}
