struct symbol *get_or_create_symbol(struct cell *cell){
    if(cell->symbol == NULL){
        cell->symbol = new_symbol();
    }
    return cell->symbol;
}

struct value_obj *get_or_create_value(struct cell *cell){
    if(cell->value == NULL){
        cell->value = new_value();
    }
    return cell->value;
}

/* this is for debugging */
int next_cell_id = 0;

struct cell *new_cell(){
   struct cell *cell = malloc(sizeof(struct cell)); 

   if(cell == NULL){
      return NULL;
   }

   memset(cell, 0, sizeof(struct cell));
   cell->id = next_cell_id++;

   return cell;
}
