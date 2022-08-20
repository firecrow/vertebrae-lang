struct closure *new_closure(struct closure *parent){
    struct closure *closure = malloc(sizeof(struct closure));
    if(closure == NULL){
        return NULL;
    }
    closure->parent = parent;
    closure->symbols = new_tree();
    if(!closure->symbols){
        return NULL;
    }
    return closure;
}

struct closure_entry *new_closure_entry(struct closure *closure, bool is_function) {
    struct closure_entry *entry = malloc(sizeof(struct closure_entry));
    if(entry == NULL){
        return NULL;
    }
    entry->closure = closure;
    entry->is_function = is_function
    return entry;
}

void closure_add_cell(struct closure *closure, struct string *key, struct cell *cell){
    struct closure_entry *entry = new_closure_entry(closure, 0);
    entry->body.cell = cell;
    tree_add(closure->symbols, key, (void*)cell);
}

void closure_add_value(struct closure *closure, struct string *key, struct value *value){
    struct closure_entry *entry = new_closure_entry(closure, 0);
    entry->body.value = value;
    tree_add(closure->symbols, key, (void*)value);
}

void closure_add_function(struct closure *closure, struct string *key, struct operator_ifc *op){
    struct closure_entry *entry = new_closure_entry(closure, 1);
    entry->body.function = op;
    tree_add(closure->symbols, key, (void *)op);
}
