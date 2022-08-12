#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <fcntl.h>

#include "ssimple.h"
#include "regex.c"
#include "value.c"
#include "string.c"
#include "symbol.c"
#include "cell.c"
#include "stack.c"
#include "parse.c"

#include "debug.c"


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
    struct cell *head = NULL;
    while(cell){
        print_space();
        print_cell(cell);
        if(head){
            print_value(head->value);
            printf("\n");
        }
        if(cell->head){
            stack = push_stack(stack, cell, head);
            head = cell->head;
            cell = cell->head;
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
