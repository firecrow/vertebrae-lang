void test_parse(){
    struct cell *start;
    struct cell *func;

    suite = new_suite("Parse tests");
    char *script = "add < 127,";

    root = parse_all(script);
    start = root->branch->branch;

    test(suite, start->value->type == SL_TYPE_SYMBOL, "add is symbol");
    test(suite, string_cmp(start->value->slot.string, str("add")) == 0, "add is the content of the symbol");
    test(suite, start->next->value->type == SL_TYPE_INT, "1 is an int");
    test(suite, start->next->value->slot.integer == 127, "1 is 127");

    script = ":hi < \"there\",";

    root = parse_all(script);

    start = root->branch;

    test(suite, start->branch->value->type == SL_TYPE_SYMBOL, "hi is symbol");
    test(suite, string_cmp(start->branch->value->slot.string, str("hi")) == 0, "hi is the symbol name");
    test(suite, string_cmp(start->branch->next->value->slot.string, str("there")) == 0, "string is the approprate value");

    script = "print < \"the sum is: \" add < 1 2 3, \" units\",";
    printf("%s\n", script);
    root = parse_all(script);

    cell = root->branch->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "print is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the content of the symbol");

    cell = cell->next;
    struct cell *level = cell;
    test(suite, cell->value->type == SL_TYPE_STRING, "second is string");
    test(suite, string_cmp(cell->value->slot.string, str("the sum is: ")) == 0, "the sum is string is the content of the string");

    cell = cell->next->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "add is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("add")) == 0, "add is the content of the symbol");

    cell = cell->next;
    test(suite, cell->value->type == SL_TYPE_INT, "1 is an integer");
    test(suite, cell->value->slot.integer == 1, "1 is 1");

    cell = cell->next;
    test(suite, cell->value->type == SL_TYPE_INT, "2 is an integer");
    test(suite, cell->value->slot.integer == 2, "2 is 2");

    cell = cell->next;
    test(suite, cell->value->type == SL_TYPE_INT, "3 is an integer");
    test(suite, cell->value->slot.integer == 3, "3 is 3");

    cell = cell->next;
    test(suite, cell == NULL, "next cell next is null");

    cell = level->next->next;
    test(suite, cell->value->type == SL_TYPE_STRING, "third section is string");
    test(suite, string_cmp(cell->value->slot.string, str(" units")) == 0, "units string is the content of the string");

    script = "print < add < 1 2 3,,";
    printf("%s\n", script);

    root = parse_all(script);

    start = root;

    cell = start->branch->branch;
    test(suite, cell->value->type == SL_TYPE_SYMBOL, "first section is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the label of the symbol");

    func = cell->next->branch;
    test(suite, string_cmp(func->value->slot.string, str("add")) == 0 , "func add is the branch");
    test(suite, func->next->value->slot.integer == 1, "next is the first value");

    script = "print << add < 1 2 3,.";
    printf("%s\n", script);

    root = parse_all(script);

    start = root;
    cell = start->branch->branch;

    test(suite, cell->value->type == SL_TYPE_SYMBOL, "first section is symbol");
    test(suite, string_cmp(cell->value->slot.string, str("print")) == 0, "print is the label of the symbol");

    func = cell->next->branch;

    test(suite, string_cmp(func->value->slot.string, str("add")) == 0 , "func add is the branch");
    test(suite, func->next->value->slot.integer == 1, "next is the first value");
    
    /* parse a function pointer */
    script = "func ->\n    print < \"hi\", print < \"there\".\nfunc < _";
    printf("%s\n", script);

    root = parse_all(script);
    print_branches(root, 0);

    start = root->branch->branch;

    test(suite, start->value->type == SL_TYPE_SYMBOL, "starts with the function symbol");
    test(suite, string_cmp(start->value->slot.string, str("func")) == 0, "first cell is func name");
    test(suite, start->value->accent = GKA_PARSE_DEF, "first cell has set accent");

    func = start->next;
    test(suite, func->value->type == SL_TYPE_CELL, "func is cell");

    struct cell *body = func->value->slot.cell;
    struct cell *part1 = body->branch;

    test(suite, string_cmp(part1->value->slot.string, str("print")) == 0, "part1 starts with print");
    test(suite, string_cmp(part1->next->value->slot.string, str("hi")) == 0, "part1 first arg is hi");

    struct cell *part2 = body->next->branch;
    test(suite, string_cmp(part2->value->slot.string, str("print")) == 0, "part2 starts with print");
    test(suite, string_cmp(part2->next->value->slot.string, str("there")) == 0, "part2 first arg is hello");

    struct cell *call = start->next->next->branch;
    test(suite, string_cmp(call->value->slot.string, str("func")) == 0, "call starts with print");
    test(suite, string_cmp(call->next->value->slot.string, str("_")) == 0, "call first arg is hello");

    summerize(suite);
}
