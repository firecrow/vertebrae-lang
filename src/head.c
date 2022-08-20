struct head *new_head(struct cell *branch, struct head *current_head){
    struct value_obj *value;

    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }

    if((value = value_if_type(branch->value, SL_TYPE_STRING)) != NULL){
        printf("\x1b[31msearching for string\x1b[0m");
        return head;
    }

    if((value = value_if_type(branch->value, SL_TYPE_FUNCTION)) != NULL){
        printf("\x1b[31msetup operator\x1b[0m");
        head->operator = value->slot.operator->new();
        return head;
    }

    return NULL;
}
