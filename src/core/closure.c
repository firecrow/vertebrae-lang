#include "../gekkota.h"

static int debug = 0;

struct closure *new_closure(struct closure *parent){
    struct closure *closure = malloc(sizeof(struct closure));
    if(closure == NULL){
        return NULL;
    }
    memset(closure, 0, sizeof(struct closure));
    closure->parent = parent;
    closure->symbols = new_tree();
    closure->lookup = closure_lookup;
    if(!closure->symbols){
        return NULL;
    }
    return closure;
}

struct value_obj *closure_lookup(struct closure *closure, struct value_obj *key){
    if(!is_string_class(key)){
        return key;
    }
    if(debug){
        printf("\x1b[36mlooking for :%s\x1b[0m\n", key->slot.string->content);
    }
    struct value_obj *result = NULL;
    while(closure && (!result || value_is_nil(result))){
        result = tree_get(closure->symbols, key->slot.string);
        closure = closure->parent;
        if(debug && closure){
            print_tree(closure->symbols);
        }
    }
    if(result){
        return result;
    }
    return key;
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

void closure_add_value(struct closure *closure, struct string *key, struct value_obj *value){
    struct closure_entry *entry = new_closure_entry(closure, 0);
    entry->body.value = value;
    tree_add(closure->symbols, key, value);
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


