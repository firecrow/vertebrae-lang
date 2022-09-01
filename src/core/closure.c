#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "core.h"

struct closure *new_closure(struct closure *parent){
    struct closure *closure = malloc(sizeof(struct closure));
    if(closure == NULL){
        return NULL;
    }
    closure->parent = parent;
    closure->symbols = new_tree();
    closure->lookup = closure_lookup;
    if(!closure->symbols){
        return NULL;
    }
    return closure;
}

struct value_obj *closure_lookup(struct closure *closure, struct value_obj *value){
    if(!is_string_class(value)){
        return value;
    }
    struct value_obj *result = NULL;
    while(closure && !result){
        printf("--------------------------\n");
        print_tree(closure->symbols);
        result = tree_get(closure->symbols, value->slot.string);
        closure = closure->parent;
    }
    return result;
}

struct closure_entry *new_closure_entry(struct closure *closure, bool is_function) {
    struct closure_entry *entry = malloc(sizeof(struct closure_entry));
    if(entry == NULL){
        return NULL;
    }
    memset(entry, 0, sizeof(struct closure_entry));

    entry->closure = closure;
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
    struct value_obj *value = new_value();
    value->type = SL_TYPE_FUNCTION;
    value->slot.operator = op;
    tree_add(closure->symbols, key, value);
    return;
}
