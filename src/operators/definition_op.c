#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "operator.h"

/* =========== definition ========== */
struct definition_operator {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    struct value_obj *value;
    struct closure *local;
    struct value_obj *key_for_next;
};

static enum SL_BRANCH_TYPE definition_handle(struct operator_ifc *_op, struct head *head, struct value_obj *value){
    printf("def op called with null?:%d\n", _op == NULL);
    struct definition_operator *op = (struct definition_operator *)_op;

    if(value && value->type == SL_TYPE_KEY){
        op->key_for_next = value;
    }else if(op->key_for_next){
        tree_add(op->local->symbols, op->key_for_next->slot.string, value);
        op->key_for_next = NULL;
    }
    print_tree(op->local->symbols);
    return SL_CONTINUE;
}

struct operator_ifc * new_definition_operator(enum OPERATOR_TYPE type) {
    struct definition_operator *op = malloc(sizeof(struct definition_operator));
    op->type = type;
    op->handle = definition_handle;
    op->new = new_definition_operator;
    op->local = new_closure(NULL);
    return (struct operator_ifc *)op;
}
