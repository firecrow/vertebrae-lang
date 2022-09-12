#include "../gekkota.h"

struct head *new_head(){
    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }
    memset(head, 0, sizeof(struct head));
    return head;
}

struct head *setup_new_head(struct head *head, struct cell *cell, struct closure *closure){
    struct value_obj *value = swap_for_symbol(closure, cell->value);

    if(value && value->type == SL_TYPE_FUNCTION){
        head->operator = value->slot.operator->new(value->slot.operator->type);
    }else if(value && value->type == SL_TYPE_CELL){
        head->operator = new_function_operator(FUNCTION);
        cell->value = value;
    }else{
        head->operator = new_default_operator(DEFAULT);
        head->value = value;
    }
    head->closure = new_closure(closure);
    head->source = value;
    head->cell = cell;
    return head;
}
