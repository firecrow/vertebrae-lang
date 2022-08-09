enum VALUE_TYPES {
    NONE = 0,
    INT = 1,
    FLOAT = 2,
    CHAR = 3,
    STRING = 4,
    CUSTOM = 5,
}

struct value_obj {
    enum VALUE_TYPES type;
    union {
        float float;
        int integer;
        struct string *string;
        char c;
        void *custome;
    } value;
}
