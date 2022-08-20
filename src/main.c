#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <fcntl.h>

#include "core/ssimple.h"
#include "types/regex.c"
#include "types/value.c"
#include "types/string.c"
#include "core/head.c"
#include "core/cell.c"
/*#include "core/closure.c"*/
#include "core/stack.c"
#include "core/parse.c"
#include "core/debug.c"



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
