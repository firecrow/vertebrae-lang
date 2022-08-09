struct string {
    char *content;
    int length;
};

struct string *new_string(){
    struct string *string = malloc(sizeof(struct string));

    if(string == NULL){
        return NULL;
    }

    memset(string, 0, sizeof(struct string));
    return string;
}

struct string *string_free(struct string *string){
    free(string->content);
    free(string);
}

int _string_resize(struct string *string, size_t length){
    ;
}

int string_append(struct string *string, struct string *additional){
    return 0; 
}

int string_append_char(struct string *string, char c){
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

