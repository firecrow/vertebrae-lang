#include "../gekkota.h"
static int debug = 1;

int head_id = 0;
struct head *new_head(){
    struct head *head = malloc(sizeof(struct head));
    if(head == NULL){
        return NULL;
    }
    memset(head, 0, sizeof(struct head));
    head->id = head_id++;
    return head;
}

struct head *setup_new_head(struct head *head, struct cell *cell, struct closure *closure){
    if(debug){
        printf("setup new head called\n");
        print_head(head);
        print_cell(cell);
    }
    struct value_obj *value = swap_for_symbol(closure, cell->value);
    if(value && value->type == SL_TYPE_FUNCTION){

        head->operator = value->slot.operator->new(value->slot.operator->type);
        head->cell = cell;
    }else if(value && value->type == SL_TYPE_CELL){

        if(debug){
            printf("\x1b[35min th magic: ");
            print_cell(value->slot.cell);
            print_cell(value->slot.cell->next);
            printf("\x1b[0m\n");
            fflush(stdout);
        }

        head->operator = new_function_operator(FUNCTION);
        cell->value = value;
        head->cell = value->slot.cell;
    }else{
        head->operator = new_default_operator(DEFAULT);
        head->value = value;
        head->cell = cell;
    }
    head->closure = new_closure(closure);
    head->source = value;

    /*
    printf("\x1b[33m");
    print_head(head);
    print_cell(head->cell->next);
    printf("\x1b[0m\n");
    */

    return head;
}
