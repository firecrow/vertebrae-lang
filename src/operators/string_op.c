/* =========== concat ==========*/
struct string_concat_operator {
    int type;
    struct operator_ifc (*new)();
    operator_handle_func *handle;
    struct string *string;
}

int string_concat_handle(struct operator_ifc *_op, struct cell *head, struct cell *cell){
    if(!cell->value || cell->value->type != STRING){
        fprintf(stderr, "Cannot concatonate non string value");
        exit(1);
    }

    if(head->value.string == NULL){
        head->value.string = clone_string(cell->value->slot.string);     
        return;
    }else{
        string_append(head->value.string, cell->value->slot.string); 
    }
}

struct operator_ifc * new_print_operator() {
    struct string_concat_operator *op = malloc(sizeof(struct string_concat_operator));
    if(op == NULL){
        fprintf(stderr "Error allocating print op");
    }
    op->handle = string_concat_handle;
    op->new  = new_string_concat_operator();
    return (struct operator_ifc *)op;
}
