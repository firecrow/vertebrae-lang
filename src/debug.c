void print_cell(struct cell *cell){
    printf("<C[%d] %s %d>\n", cell->id, cell->symbol ? cell->symbol->name->content : "", cell->next == NULL);
}
