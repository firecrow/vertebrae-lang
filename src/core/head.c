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
    memset(head, 0, sizeof(struct head));

    struct closure *current = NULL;
    if(current_head && current_head->closure)
        current = current_head->closure;
    head->closure = new_closure(current);


    if(!branch){
        return head;
    }

    if(is_type(branch->value, SL_TYPE_STRING)){
        printf("nope");
        return head;
    }

    if(is_type(branch->value, SL_TYPE_SYMBOL)){
        if(current_head){
            struct closure *closure = current_head->closure;
            if(closure){
                struct value_obj *result = closure->lookup(closure, branch->value->slot.string);
                if(result && result->type == SL_TYPE_FUNCTION){
                    head->operator = result->slot.operator;
                }
                return head;
            }
        }
    }

    if(is_type(value, SL_TYPE_FUNCTION)){
        enum OPERATOR_TYPE optype = value->slot.operator->type;
        head->operator = value->slot.operator->new(optype);
        return head;
    }

    return NULL;
}
