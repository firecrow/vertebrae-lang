#include "external.h"

#include "ssimple.h"
#include "core/core.h"
#include "types/types.h"
#include "operators/operator.h"
#include "library/library.h"

int spacing = 0;

void print_space(){
    for(int i =0; i< spacing; i++){
        write(STDOUT, " ", 1);
    }
}

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);

    printf("Root is null? %d\n", root == NULL);
    struct stack_item *stack = NULL;

    struct cell *cell = root;
    struct head *head = new_head(NULL, NULL);

    init_basic_library(head->closure); 

    struct closure_entry *entry;
    struct operator_ifc *op  = NULL;
    while(cell){
        print_space();
        print_cell(cell);

        if(head){
            print_value(head->value);
            if(head->operator){
                head->operator->handle(head->operator, head, cell);
            }
        }

        if(cell->branch){
            /* creating the head will effectively process the cell */
            head = new_head(cell->branch, head);
            stack = push_stack(stack, cell, head);
            cell = cell->branch;

            print_space();
            print_cell(cell);

            spacing += 4;
        }

        if(cell->next){
            cell = cell->next;
        }else{
            cell = NULL;
            while(stack && cell == NULL){
                cell = stack->cell->next;
                head = stack->head;
                stack = stack->previous;
                spacing -= 4;
            }
        }
    }
    return 0;
}
