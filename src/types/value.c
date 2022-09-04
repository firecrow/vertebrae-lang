#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "types.h"

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

bool is_string_class(struct value_obj *value){
    if(!value){
        return 0;
    }
    return value->type == SL_TYPE_SYMBOL || 
        value->type == SL_TYPE_STRING ||
        value->type == SL_TYPE_QUOTE ||
        value->type == SL_TYPE_KEY;
}

bool is_type(struct value_obj *value, enum SL_TYPE type){
    return value && value->type == type;
}

struct value_obj *clone_value(struct value_obj *value){
   /* TODO: not yet implemented */
   return value;
}

static struct string *int_to_string(struct value_obj *value){
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Type not int for int to string");
    }
    struct string *s = new_string_xprintf("%d", value->slot.integer);
    return s;
}

static struct string *string_to_string(struct value_obj *value){
    if(value->type != SL_TYPE_STRING){
        fprintf(stderr, "Type not string for int to string\n");
    }
    return value->slot.string;
}

struct value_obj *new_string_value_obj(struct string *string){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_STRING;
    value->slot.string = string;
    value->to_string = string_to_string;
    return value;
}

struct value_obj *new_symbol_value_obj(struct string *string){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_SYMBOL;
    value->slot.string = string;
    value->to_string = string_to_string;
    return value;
}

struct value_obj *new_int_value_obj(int intval){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_INT;
    value->slot.integer = intval;
    value->to_string = int_to_string;
    return value;
}

struct value_obj *value_from_token(enum SL_PARSE_STATE state, struct string *token){

    if(state == IN_STRING){
        return new_string_value_obj(token);
    }

    if(state == IN_QUOTE){
        struct value_obj *value = new_string_value_obj(token);
        value->type = SL_TYPE_QUOTE;
        return value;
    }

    if(state == IN_KEY){
        struct value_obj *value = new_string_value_obj(token);
        value->type = SL_TYPE_KEY;
        return value;
    }

    struct value_obj *value = new_value();
    if(state == IN_COMMENT){
        value->type = SL_TYPE_COMMENT;
        value->slot.string = token;
        value->to_string = string_to_string;
        return value;
    }

    if(regex_match("^[0-9]\\+$", token)){
        value->type = SL_TYPE_INT;
        value->slot.integer = atoi(token->content);
        value->to_string = int_to_string;
        return value;
    }

    value->type = SL_TYPE_SYMBOL;
    value->slot.string = token;

    return value;
}

struct value_obj *swap_for_symbol(struct closure *closure, struct value_obj *value){
    struct value_obj *result = closure->lookup(closure, value);
    if(result){
        return result;
    }
    return value;
}


