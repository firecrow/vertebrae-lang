#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../operators/operator.h"
#include "core.h"

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
        head->operator = value->slot.operator;
    }
    head->closure = closure;
    return head;
}
