struct head *new_head(struct cell *branch){
    struct value_obj *value;
    if((value = value_if_type(branch->value, SL_TYPE_FUNCTION)) == NULL){
        fprintf(stderr, "Error TYPE mismatch expected %d got %d", SL_TYPE_FUNCTION, branch->value->type);
        return NULL;
    }
    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }
    head->operator = value->value.operator->new();
    return head;
}
