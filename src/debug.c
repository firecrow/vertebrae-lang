
char *debug_SL_TYPE[] = {
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
    "COMMENT"
};



void print_value(struct value_obj *value){
    if(value->type == SL_TYPE_SYMBOL){
        printf("\x1b[33m%s\x1b[0m", value->value.string->content);
    }
    if(value->type == SL_TYPE_STRING){
        printf("\x1b[35m\"%s\"\x1b[0m", value->value.string->content);
    }
    if(value->type == SL_TYPE_INT){
        printf("\x1b[36m%d\x1b[0m", value->value.integer);
    }
    if(value->type == SL_TYPE_COMMENT){
        printf("\x1b[34m%s\x1b[0m", value->value.string->content);
    }

    printf(" %s%d", debug_SL_TYPE[value->type], value->id);
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
