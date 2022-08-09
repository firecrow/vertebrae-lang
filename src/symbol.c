struct symbol {
   struct string *name; 
   struct cell *cell;
};

struct symbol *new_symbol(){

    struct symbol *symbol = malloc(sizeof(struct symbol));

    if(symbol == NULL){
        return NULL;
    }

    memset(symbol, 0, sizeof(struct symbol));

    return symbol;
}
