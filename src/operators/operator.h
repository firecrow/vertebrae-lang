enum OPERATOR_TYPE {
    CUSTOM = 0,
    CONDITION,
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,
    PRINT,
    DEFINITION,
    CLOSURE 
};

enum SL_BRANCH_TYPE {
    SL_CONTINUE =0,
    SL_BREAK
};

typedef enum SL_BRANCH_TYPE (operator_handle_func)(struct operator_ifc *_op, struct crw_state *ctx);

struct operator_ifc {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

struct operator_ifc * new_definition_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_print_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_closure_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_equal_operator(enum OPERATOR_TYPE type);
