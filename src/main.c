#include "external.h"

#include "ssimple.h"
#include "core/core.h"
#include "types/types.h"

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
    struct head *head = NULL;
    struct closure_entry *entry;
    struct operator_ifc *op  = NULL;
    while(cell){
        print_space();
        print_cell(cell);
        if(head){
            printf("head value: ");
            print_value(head->value);
            printf("\n");
        }
        if(cell->branch){
            head = new_head(cell->branch, head);
            stack = push_stack(stack, cell, head);
            cell = cell->branch;
            spacing += 4;
        }else if(cell->next){
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
