#include "../gekkota.h"

static char *debug_SL_TYPE[] = {
    "SL_TYPE_NONE",
    "SL_TYPE_BOOLEAN_RESULT",
    "SL_TYPE_CELL",
    "SL_TYPE_CHAR",
    "SL_TYPE_CLOSURE",
    "SL_TYPE_COMMENT",
    "SL_TYPE_FLOAT",
    "SL_TYPE_FUNCTION",
    "SL_TYPE_HEAD",
    "SL_TYPE_INT",
    "SL_TYPE_KEY",
    "SL_TYPE_QUOTE",
    "SL_TYPE_SET_LEX",
    "SL_TYPE_STRING",
    "SL_TYPE_SYMBOL",
    "SL_TYPE_TREE",
    "SL_TYPE_VALUE"
};

void print_value(struct value_obj *value){
    if(!value){
        printf("NULL");
        return;
    }
    if(value->accent == GKA_PARSE_QUOTE){
        printf("'");
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
        printf(".%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_QUOTE){
        printf("'%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_COMMENT){
        printf("%s", value->slot.string->content);
    }
    if(value->type == SL_TYPE_BOOLEAN_RESULT){
        printf("%s", value->to_string(value)->content);
    }

    printf(" %s %d %d", debug_SL_TYPE[value->type], value->id, value->accent);
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
    printf("C(n%d/b%d h%d", 
        cell->next != NULL ? cell->next->id : -1, 
        cell->branch != NULL ? cell->branch->id : -1,
        cell->is_head);
    printf(")>");
    fflush(stdout);
}

void print_head(struct head *head){
    if(!head){
        printf("<H NULL>");
        return;
    }
    printf("<H%d ", head->id);
    printf(" source value:");
    print_value(head->source);
    if(head->operator){
        printf(" opp type %d ", head->operator->type);
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
    printf("<tree %d\n", tree->id);
    struct order_entry *oentry = tree->order;
    while(oentry){
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


void print_branches(struct cell *cell, int indent){
    while(cell){
        int show = indent;
        while(show-- > 0){
            printf("    ");
        }
        print_cell(cell);
        printf("\n");
        if(cell->value && cell->value->type == SL_TYPE_CELL){
            printf(" ->\n");
            print_branches(cell->value->slot.cell, indent);
            printf("\n");
        }
        if(cell->branch){
            print_branches(cell->branch, indent+1);
        }
        cell = cell->next;
    }
}
