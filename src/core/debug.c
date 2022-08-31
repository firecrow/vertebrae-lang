#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../operators/operator.h"
#include "core.h"

static char *debug_SL_TYPE[] = {
    "NONE",
    "SYMBOL",
    "INT",
    "FLOAT",
    "CHAR",
    "STRING",
    "TREE",
    "CELL",
    "CLOSURE",
    "FUNCTION",
    "VALUE",
    "COMMENT",
    "QUOTE",
    "KEY"
};

void print_value(struct value_obj *value){
    if(!value || !value->type){
        printf("\x1b[33mNONE\x1b[0m");
        return;
    }
    if(value->type == SL_TYPE_SYMBOL){
        printf("\x1b[33m%s\x1b[0m", value->slot.string->content);
    }
    if(value->type == SL_TYPE_STRING){
        printf("\x1b[35m\"%s\"\x1b[0m", value->slot.string->content);
    }
    if(value->type == SL_TYPE_KEY){
        printf("\x1b[32m:%s\x1b[0m", value->slot.string->content);
    }
    if(value->type == SL_TYPE_QUOTE){
        printf("\x1b[0m'%s\x1b[0m", value->slot.string->content);
    }
    if(value->type == SL_TYPE_INT){
        printf("\x1b[36m%d\x1b[0m", value->slot.integer);
    }
    if(value->type == SL_TYPE_COMMENT){
        printf("\x1b[34m%s\x1b[0m", value->slot.string->content);
    }

    printf(" %s %d", debug_SL_TYPE[value->type], value->id);
}

void print_cell(struct cell *cell){
    printf("<C%d ", cell->id);
    if(cell->value){
        print_value(cell->value);
    }
    printf("C(n%d/h%d", cell->next != NULL, cell->branch != NULL);
    printf(")\n");
    fflush(stdout);
}

void print_head(struct head *head){
    printf("<H");
    if(head->operator){
        printf(" %d", head->operator->type);
    }
    printf(">\n");
    fflush(stdout);
}

void print_tree(struct tree *tree){
    struct order_entry *oentry = tree->order;
    while(oentry){
        printf("key:%s", oentry->entry->key->content);
        print_value(oentry->entry->content);
        printf("\n");
        oentry = oentry->next;
    }
}
