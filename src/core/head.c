#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../operators/operator.h"
#include "core.h"

struct head *new_head(struct cell *branch, struct head *current_head){
    struct value_obj *value;

    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }

    if((value = value_if_type(branch->value, SL_TYPE_STRING)) != NULL){
        printf("\x1b[31msetting value for string\x1b[0m");
        return head;
    }

    if((value = value_if_type(branch->value, SL_TYPE_STRING)) != NULL){
        printf("\x1b[31msearching for string\x1b[0m");
        return head;
    }

    if((value = value_if_type(branch->value, SL_TYPE_FUNCTION)) != NULL){
        printf("\x1b[31msetup operator\x1b[0m");
        enum OPERATOR_TYPE optype = value->slot.operator->type;
        head->operator = value->slot.operator->new(optype);
        return head;
    }

    return NULL;
}
