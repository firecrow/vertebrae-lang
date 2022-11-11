#include "../gekkota.h"

struct stack_item *new_stack_item(struct stack_item *existing, struct cell *cell, struct head *head){
    struct stack_item *stack_item = malloc(sizeof stack_item);
    if(stack_item == NULL){
        fprintf(stderr, "Error allocating stack item");
        exit(1);
    }
    memset(stack_item, 0, sizeof(struct stack_item));

    stack_item->cell = cell;
    stack_item->head = head;
    stack_item->previous = existing;
    return stack_item;
}


