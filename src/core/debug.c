#include "../external.h"
#include "../ssimple.h"
#include "../types/types.h"
#include "../operators/operator.h"
#include "../run/run.h"
#include "core.h"

static char *debug_SL_TYPE[] = {
    "SL_TYPE_NONE",
    "SL_TYPE_BOOLEAN_RESULT",
    "SL_TYPE_SYMBOL",
    "SL_TYPE_INT",
    "SL_TYPE_FLOAT",
    "SL_TYPE_CHAR",
    "SL_TYPE_STRING",
    "SL_TYPE_TREE",
    "SL_TYPE_CELL",
    "SL_TYPE_CLOSURE",
    "SL_TYPE_FUNCTION",
    "SL_TYPE_VALUE",
    "SL_TYPE_COMMENT",
    "SL_TYPE_QUOTE",
    "SL_TYPE_KEY"
};

void print_value(struct value_obj *value){
    if(!value){
        printf("NULL");
        return;
    }
    if(!value->type){
        printf("NONE");
        return;
    }
    if(value->type == SL_TYPE_SYMBOL){
        printf("%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_STRING){
        printf("\"%s\"", value->slot.string->content);
    }
    if(value->type == SL_TYPE_INT){
        printf("%d", value->slot.integer);
    }
    if(value->type == SL_TYPE_KEY){
        printf(":%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_QUOTE){
        printf("'%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_COMMENT){
        printf("%s", value->slot.string->content);
    }

    printf(" %s %d", debug_SL_TYPE[value->type], value->id);
}

void print_cell(struct cell *cell){
    if(!cell){
        printf("C(NULL)");
        return;
    }
    printf("<C%d ", cell->id);
    if(cell->value){
        print_value(cell->value);
    }
    printf("C(n%d/b%d", cell->next != NULL, cell->branch != NULL);
    printf(")>");
    fflush(stdout);
}

void print_head(struct head *head){
    printf("<H");
    printf(" source value:");
    print_value(head->source);
    if(head->operator){
        printf(" opp type %d", head->operator->type);
    }else{
        printf(" no op ");
    }
    printf(" head value: ");
    print_value(head->value);
    printf(">");
    fflush(stdout);
}

void print_space(int indent){
    for(int i = 0; i < indent; i ++){
        printf("    ");
    }
}

void print_state(struct crw_state *ctx, bool show_tree){
    print_space(ctx->nesting);
    printf("[[[ ");
    print_head(ctx->head);
    printf(" - ");
    print_cell(ctx->cell);
    if(show_tree){
        print_tree(ctx->head->closure->symbols);
    }
    printf("]]]");
    fflush(stdout);
}

void print_tree(struct tree *tree){
    struct order_entry *oentry = tree->order;
    while(oentry){
        printf("");
        if(oentry->entry){
            if(oentry->entry->key){
                printf("    %s:", oentry->entry->key->content);
            }
            print_value(oentry->entry->content);
            printf("\n");
        }
        oentry = oentry->next;
    }
}
