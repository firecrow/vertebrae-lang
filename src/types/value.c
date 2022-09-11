#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "types.h"

/* this is for debugging */
int next_value_id = 0;

/*----- univeral -----*/

static bool truthy_true(struct value_obj *value){
    return 1;
}

static bool equals_false(struct value_obj *source, struct value_obj *compare){
    return 0;
}

bool is_type(struct value_obj *value, enum SL_TYPE type){
    return value && value->type == type;
}

struct value_obj *clone_value(struct value_obj *value){
   /* TODO: not yet implemented */
   if(value->type = SL_TYPE_INT){
        return new_int_value_obj(value->slot.integer);
   }
   return value;
}

static struct string *default_to_string(struct value_obj *value){
    return str("<Value>");
}

struct value_obj *new_value(){
   struct value_obj *value = malloc(sizeof(struct value_obj)); 

   if(value == NULL){
      return NULL;
   }

   memset(value, 0, sizeof(struct value_obj));
   value->id = next_value_id++;
   value->truthy = truthy_true;
   value->equals = equals_false;
   value->to_string = default_to_string;

   return value;
}

/*----- string -----*/

bool is_string_class(struct value_obj *value){
    if(!value){
        return 0;
    }
    return value->type == SL_TYPE_SYMBOL || 
        value->type == SL_TYPE_STRING ||
        value->type == SL_TYPE_QUOTE ||
        value->type == SL_TYPE_KEY;
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

/*----- result -----*/

static bool result_equals(struct value_obj *source, struct value_obj *compare){
    if(source->type != SL_TYPE_BOOLEAN_RESULT){
        return 0;
    }
    if(source->type != compare->type){
        return 0;
    }

    return source->slot.result == compare->slot.result;
}

bool value_is_nil(struct value_obj *value){
    return value && value->type == SL_TYPE_BOOLEAN_RESULT && value->slot.result == NIL;
}

static bool result_truthy(struct value_obj *value){
    if(value->type != SL_TYPE_BOOLEAN_RESULT){
        fprintf(stderr, "Error result_truthy called on non result type\n");
        return 0;
    }
    return value->slot.result == TRUE;
} 

static struct string *result_to_string(struct value_obj *value){
    if(value->type != SL_TYPE_BOOLEAN_RESULT){
        fprintf(stderr, "Type not string for int to string\n");
    }
    enum CRW_RESULT result = value->slot.result;
    if(result == TRUE){
       return str("true"); 
    }
    if(result == FALSE){
       return str("false"); 
    }
    if(result == NIL){
       return str("nil"); 
    }
    if(result == ERROR){
       return str("error"); 
    }
    return value->slot.string;
}

struct value_obj *new_result_value_obj(enum CRW_RESULT result){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_BOOLEAN_RESULT;
    value->slot.result = result;
    value->to_string = result_to_string;
    value->truthy = result_truthy;
    value->equals = result_equals;
    return value;
}


/*----- int -----*/


static struct string *int_to_string(struct value_obj *value){
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Type not int for int to string");
    }
    struct string *s = new_string_xprintf("%d", value->slot.integer);
    return s;
}


static struct string *int_truthy(struct value_obj *value){
    if(value->type != SL_TYPE_INT){
        fprintf(stderr, "Type not int for int to string");
    }
    return value->slot.integer != 0;
}

static bool int_equals(struct value_obj *source, struct value_obj *compare){
    if(source->type != SL_TYPE_INT){
        return 0;
    }
    if(source->type != compare->type){
        return 0;
    }

    return source->slot.integer == compare->slot.integer;
}

struct value_obj *new_int_value_obj(int intval){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_INT;
    value->slot.integer = intval;
    value->to_string = int_to_string;
    value->truthy = int_truthy;
    value->equals = int_equals;
    return value;
}

/*----- cell -----*/
static struct string *cell_to_string(struct value_obj *value){
    return str("<cell>");
}

struct value_obj *new_cell_value_obj(struct cell *cell){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_CELL;
    value->slot.cell = cell;
    value->to_string = cell_to_string;
    return value;
}

/*----- symbol -----*/
static struct string *symbol_to_string(struct value_obj *value){
    if(value->type != SL_TYPE_SYMBOL){
        fprintf(stderr, "Type not string for int to string\n");
    }
    struct string *output = str("<symbol:");
    string_append(output, value->slot.string);
    string_append(output, str(">"));
    return output;
}

struct value_obj *new_symbol_value_obj(struct string *string){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_SYMBOL;
    value->slot.string = string;
    value->to_string = symbol_to_string;
    return value;
}

/*----- key -----*/

struct value_obj *new_key_value_obj(struct string *string){
    struct value_obj *value = new_value();
    value->type = SL_TYPE_KEY;
    value->slot.string = string;
    value->to_string = string_to_string;
    return value;
}


/*----- factories/utils -----*/

bool is_non_head_class(struct value_obj *value){
    if(!value){
        return 0;
    }
    return value->type == SL_TYPE_NONE || 
        value->type == SL_TYPE_KEY;
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
        return new_int_value_obj(atoi(token->content));
    }

    return new_symbol_value_obj(token);
}

struct value_obj *swap_for_symbol(struct closure *closure, struct value_obj *value){
    if(!value || value->type != SL_TYPE_SYMBOL){
        return value;
    }
    struct value_obj *prior = NULL;
    while(prior != value && value->type == SL_TYPE_SYMBOL){
        prior = value;
        struct value_obj *result = closure->lookup(closure, value);
        if(result){
            value = result;
        }
    }
    return value;
}


