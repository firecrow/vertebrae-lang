/* this is for debugging */
int next_value_id = 0;

struct value_obj *new_value(){
   struct value_obj *value = malloc(sizeof(struct value_obj)); 

   if(value == NULL){
      return NULL;
   }

   memset(value, 0, sizeof(struct value_obj));
   value->id = next_value_id++;

   return value;
}

struct value_obj *clone_value(struct value *value){
   /* TODO: not yet implemented */
   return value;
}

struct value_obj *value_from_token(enum SL_PARSE_STATE state, struct string *token){
    struct value_obj *value = new_value();

    if(state == IN_STRING){
        value->type = SL_TYPE_STRING;
        value->value.string = token;
        return value;
    }

    if(state == IN_COMMENT){
        value->type = SL_TYPE_COMMENT;
        value->value.string = token;
        return value;
    }

    if(regex_match("^[0-9]\\+$", token)){
        value->type = SL_TYPE_INT;
        value->value.integer = atoi(token->content);
        return value;
    }

    value->type = SL_TYPE_SYMBOL;
    value->value.string = token;
    return value;
}
