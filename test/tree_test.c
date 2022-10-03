
void test_tree(){
    struct suite *suite = NULL;
    struct order_entry *order_entry = NULL;
    struct value_obj *result = NULL;

/***************** Tree Suite *************/
    suite = new_suite("Tree");
    struct tree *tree = new_tree();

    struct value_obj *alpha = new_string_value_obj(str("apples"));
    struct string *alpha_key = str("alpha");

    struct value_obj *bravo = new_string_value_obj(str("berries"));
    struct string *bravo_key = str("bravo");

    struct value_obj *charlie = new_string_value_obj(str("charlie"));
    struct string *charlie_key = str("carrots");

    tree_add(tree, alpha_key, alpha);
    test(suite, tree->count == 1, "Tree count is 1 for first record");

    result = tree_get(tree, alpha_key);
    test(suite, !strncmp(result->slot.string->content, alpha->slot.string->content, 4096), "Retrieved result matches"); 
    
    test(suite, tree->order->entry->content == alpha , "Order starts with first record"); 

    tree_add(tree, bravo_key, bravo);
    test(suite, tree->count == 2, "Tree count is 2 for second record");

    order_entry = tree->order;
    test(suite, order_entry->next->entry->content == bravo, "Second order points to second record"); 

    test(suite, order_entry->next->previous == order_entry, "Order entry is double linked"); 

    result = tree_get(tree, bravo_key);
    test(suite, !strncmp(result->slot.string->content, bravo->slot.string->content, 4096), "Retrieved result matches"); 

    tree_add(tree, charlie_key, charlie);
    test(suite, tree->count == 3, "Tree count is 3 for third record");

    result = tree_get(tree, charlie_key);
    test(suite, !strncmp(result->slot.string->content, charlie->slot.string->content, 4096), "Retrieved result matches"); 

    summerize(suite);
}
