#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "types.h"

struct string *new_string(){
    struct string *string = malloc(sizeof(struct string));

    if(string == NULL){
        return NULL;
    }

    memset(string, 0, sizeof(struct string));

    string->content = malloc(STRING_DEFAULT_SIZE);
    if(string->content == NULL){
        return NULL;
    }
    memset(string->content, 0, sizeof(STRING_DEFAULT_SIZE));
    string->allocated = STRING_DEFAULT_SIZE;
    
    return string;
}

struct string *clone_string(struct string *string){
    struct string *new = new_string();
    if(new == NULL){
        fprintf(stderr, "Error allocating string for clone");
        exit(1);
    }
    new->length = string->length;
    new->content = malloc(sizeof(string->length));
    if(new->content == NULL){
        fprintf(stderr, "Error allocating string for clone");
        exit(1);
    }
    memcpy(new->content, string->content, string->length);
    
    return new;
}

struct string *string_free(struct string *string){
    free(string->content);
    free(string);
}

int _string_resize(struct string *string, size_t length){
    if(string->allocated < length){
        while(string->allocated < length+1){
            string->allocated = string->allocated * 2;
        }
        string->content = realloc(string->content, string->allocated);
        if(string->content == NULL){
            fprintf(stderr, "String reallocation in resize failed, aborting");
            exit(1);
        }
    }
}

int string_append(struct string *string, struct string *additional){
    _string_resize(string, string->length+additional->length);
    memcpy(string->content+(string->length-1), additional->content, additional->length);
    string->length = string->length+additional->length;
    string->content[string->length] = '\0';
    return string->length; 
}

int string_append_char(struct string *string, char c){
    fflush(stdout);
    _string_resize(string, string->length+1);
    string->content[string->length] = c;
    string->length++;
    string->content[string->length] = '\0';
    return 0; 
}

struct string *string_from_cstring(char *cstring){
    size_t length = strlen(cstring);
    
    struct string *string = new_string();

    if(string == NULL){
        char msg[] = "Error allocating string aborting";
        exit(1);
    }

    string->content = malloc(length);
    if(string->content == NULL){
        char msg[] = "Error allocating string aborting";
        exit(1);
    }

    memcpy(string->content, cstring, length);

    string->length = length;

    return string;
}

struct string* new_string_xprintf(const char *restrict format, ...){
  /* save the args to for later becuase we need to do two passes */
  va_list args;
  va_list args_copy;
  va_start(args, format);
  va_copy(args_copy, args);

  /* make a 1 character string with the intention of it 
   * failing and telling us how long in _should_ be
   */
  char x[] = " ";
  int length = vsnprintf(x, 1, format, args)+1;
  /* allocate the proper amount */
  char *content = malloc(length+1);
  if(content == NULL){
    fprintf(stderr, "Error allocating string in xprintf");
    exit(1);
  }

  /* call the print function with the copy of the args
   * and the proper length
   */
  vsnprintf(content, length, format, args_copy);
  content[length] = '\0';  

  va_end(args);
  va_end(args_copy);

  struct string *string = new_string();
  string_append(string, string_from_cstring(content));

  return string;
}
