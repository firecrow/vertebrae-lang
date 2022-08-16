struct head *new_head(struct operator_ifc *op){
    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }
    head->op = op->new();
    return head;
}
