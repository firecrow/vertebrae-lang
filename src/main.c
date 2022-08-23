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

void passthrough(struct head *head, struct head *previous){
    if(head && head->operator && previous && previous->operator && previous->value){
        head->operator->handle(head->operator, head, previous->value);
    }
}

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);

    struct stack_item *stack = NULL;

    enum SL_BRANCH_TYPE = branch_type;

    struct cell *cell = root;
    struct head *head = new_head(NULL, NULL);
    struct head *previous_head = head;

    init_basic_library(head->closure); 

    struct closure_entry *entry;
    struct operator_ifc *op  = NULL;
    while(cell){
        /*
        print_space();
        print_cell(cell);
        */

        if(cell->branch){
            /* creating the head will effectively process the cell */
            head = new_head(cell->branch, head);
            stack = push_stack(stack, cell, head);
            cell = cell->branch;

            /*
            print_space();
            print_cell(cell);
            */

            spacing += 4;
        }else if(head){
            if(head->operator){
                branch_type = head->operator->handle(head->operator, head, cell->value);
                /* if the handle has communicated that it no longer wants to 
                 * run the rest of the cells, setting cell->next to NULL here
                 * will cause the if/else branch following to pull from the
                 * previous stack entry
                 */
                if(branch_type == SL_BREAK){
                    cell->next = NULL;
                }
            }
        }

        if(cell->next){
            cell = cell->next;
        }else{
            cell = NULL;
            if(head){
                previous_head = head;
            }
            head = NULL;
            stack = stack->previous;
            while(stack && cell == NULL){
                cell = stack->cell->next;
                head = stack->head;
                stack = stack->previous;
                passthrough(head, previous_head);
                spacing -= 4;
            }
        }
    }
    return 0;
}
