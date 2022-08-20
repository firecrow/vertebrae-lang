enum ARITHMETIC_TYPS {
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY
}

struct arithmetic_operator {
    int type;
    struct operator_ifc (*new)();
    operator_handle_func *handle;
    struct value *value;
    enum ARITHMATIC_TYPE arithmatic_type;
    struct (*operation)(struct arithmetic_operator, struct value *new);
}

int arithmatic_handle(struct operator_ifc *_op, struct cell *head, struct cell *cell){
    if(cell->value->type != INTEGER){
        fprintf(stderr, "Cannot do arithmetic on non integer value");
        exit(1);
    }
    struct arithmatic_operator *op = (arithmatic_operator*)op;
    if(!head->value){
        head->value = clone_value(cell->value);
        return;
    }

    int new_value = cell->value->slot.integer;
    if(op->arithmatic_type == ADD){
        head->value += new_value;
    }else if(op->arithmatic_type == SUBTRACT){
        head->value -= new_value;
    }else if(op->arithmatic_type == DIVIDE){
        head->value =  head->value / new_value;
    }else if(op->arithmatic_type == MULTIPLY){
        head->value *= new_value;
    }
}

struct operator_ifc * new_arithmetic_operator(enum ARITHMETIC_TYPE type) {
    struct arithmatic_operator *op = malloc(sizeof(struct arithmatic_operator));
    op->arithmatic_type = type;
    op->handle = arithmatic_handle;
    op->new = new_arithmatic_operator;
    return (struct operator_ifc *)op;
}
