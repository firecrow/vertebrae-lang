enum OPERATOR_TYPE {
    CUSTOM = 0,
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,
    PRINT
};

enum SL_BRANCH_TYPE {
    SL_CONTINUE =0,
    SL_BREAK
};

typedef enum SL_BRANCH_TYPE (operator_handle_func)(struct operator_ifc *_op, struct head *head, struct value_obj *value);

struct operator_ifc {
    int type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_print_operator(enum OPERATOR_TYPE type);
