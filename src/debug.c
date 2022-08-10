
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
    printf("V[%d](%s)", value->id, debug_SL_TYPE[value->type]);
    if(value->type == SL_TYPE_SYMBOL){
        printf("=%s", value->value.string->content);
    }
    if(value->type == SL_TYPE_STRING){
        printf("=\"%s\"", value->value.string->content);
    }
    if(value->type == SL_TYPE_INT){
        printf("=%d", value->value.integer);
    }
    if(value->type == SL_TYPE_COMMENT){
        printf("=%s", value->value.string->content);
    }
}

void print_cell(struct cell *cell){
    printf("<C[%d] %s %d ", cell->id, cell->symbol ? cell->symbol->name->content : "", cell->next == NULL);
    if(cell->value){
        print_value(cell->value);
    }
    printf(">\n");
    fflush(stdout);
}
