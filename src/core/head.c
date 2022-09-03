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

void setup_head(struct head *head, struct cell *cell, struct head *closure){
    struct value_obj *value = swap_for_symbol(closure, cell->value);

    struct closure *current = NULL;

    if(is_type(value, SL_TYPE_SYMBOL)){
        if(value && value->type == SL_TYPE_FUNCTION){
            head->operator = value->slot.operator;
        }
        return head;
    }

    return NULL;
}
