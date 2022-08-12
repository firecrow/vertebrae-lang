struct stack_item *new_stack_item(struct stack_item *existing, struct cell *cell, struct cell *head){
    struct stack_item *stack_item = malloc(sizeof stack_item);
    if(stack_item == NULL){
        fprintf(stderr, "Error allocating stack item");
        exit(1);
    }
    stack_item->cell = cell;
    stack_item->head = head;
    stack_item->previous = existing;
    return stack_item;
}

struct stack_item *push_stack(struct stack_item *existing, struct cell *cell, struct cell *head){
    struct stack_item *item = new_stack_item(existing, cell, head);
    return item;
}
