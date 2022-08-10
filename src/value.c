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

struct value_obj *value_from_token(enum SL_PARSE_STATE state, struct string *token){
    struct value_obj *value = new_value();
    struct string *int_pattern = string_from_cstring("^[0-9]+$");

    if(state == IN_STRING){
        value->type = SL_TYPE_STRING;
        value->value.string = string_from_cstring(token->content);
    }

    if(regex_match(int_pattern, token)){
        value->type = SL_TYPE_INT;
        value->value.integer = atoi(token->content);
        return value;
    }

    value->type = SL_TYPE_SYMBOL;
    value->value.string = string_from_cstring(token->content);
    return value;
}
