/* =========== definition ========== */
struct definition_operator {
    int type;
    operator_ifc (*new)();
    operator_handle_func *handle;
    struct value_obj *value;
    bool is_function;
}

int definition_handle(struct operator_ifc *_op, struct cell *head, struct cell *cell){
    struct value_obj *value = cell->value;
    if(is_branch_cell(cell)){
        value = new_value();
        value->type = CELL;
        value->slot.cell = cell;
    }
    op->value = clone_value(value);
    return 0;
}

struct operator_ifc * new_function_definition_operator() {
    op = malloc(sizeof(struct definition_operator));
    op->handle = definition_handle;
    op->is_function = 1;
    return (struct operator_ifc *)op;
}

struct operator_ifc * new_definition_operator() {
    op = malloc(sizeof(struct definition_operator));
    op->handle = definition_handle;
    definition_singletn->new = new_definition_operator();
    return (struct operator_ifc *)op;
}
