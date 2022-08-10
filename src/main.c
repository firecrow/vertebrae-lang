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
#include "parse.c"

#include "debug.c"

struct stack_item {
    struct cell *cell;
    struct stack_item *previous;
};

struct stack_item *new_stack_item(struct stack_item *existing, struct cell *cell){
    struct stack_item *stack_item = malloc(sizeof stack_item);
    if(stack_item == NULL){
        fprintf(stderr, "Error allocating stack item");
        exit(1);
    }
    stack_item->cell = cell;
    stack_item->previous = existing;
    return stack_item;
}

struct stack_item *push_stack(struct stack_item *existing, struct cell *cell){
    struct stack_item *item = new_stack_item(existing, cell);
    return item;
}

int spacing = 0;

void print_space(){
    for(int i =0; i< spacing; i++){
        write(STDOUT, " ", 1);
    }
}

int main(int argc, char *argv[]) {
    int source = open(argv[1], O_RDONLY);
    struct cell *root = parse_file(source);
    char msg[] = "Root is null?";

    printf("Root is null? %d\n", root == NULL);
    struct stack_item *stack = NULL;

    struct cell *cell = root;
    while(cell){
        print_space();
        print_cell(cell);
        if(cell->branch){
            spacing += 4;
            stack = push_stack(stack, cell);
            cell = cell->branch;
        } else if(cell->next){
            cell = cell->next;
        } else if(stack){
            cell = NULL;
            while(cell == NULL && stack){
                printf("stack\n");
                print_cell(stack->cell);
                if(stack->cell->branch){
                    spacing += 4;
                    cell = stack->cell->branch;
                    stack = push_stack(stack, cell);
                }else if(stack->cell->next){
                    cell = stack->cell->next;
                }
                spacing -= 4;
                stack = stack->previous;
            }
        } else {
            cell = NULL;
        }
    }
    return 0;
}
