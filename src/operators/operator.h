enum OPERATOR_TYPE {
    CUSTOM = 0,
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY
};

typedef void (operator_handle_func)(struct operator_ifc *_op, struct head *head, struct cell *cell);

struct operator_ifc {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

