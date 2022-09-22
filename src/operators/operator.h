enum OPERATOR_TYPE {
    CUSTOM = 0,
    DEFAULT,
    CONTEXT,
    CONDITION,
    EQUAL,
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,
    PRINT,
    DEFINITION,
    CLOSURE,
    FUNCTION,
    SET
};

enum SL_BRANCH_TYPE {
    SL_CONTINUE =0,
    SL_BREAK
};

typedef void (operator_handle_func)(struct operator_ifc *_op, struct crw_state *ctx);

struct operator_ifc {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
};

void default_next(struct crw_state *ctx);

struct operator_ifc * new_definition_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_arithmetic_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_print_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_closure_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_condition_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_equal_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_default_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_function_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_set_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_context_operator(enum OPERATOR_TYPE type);
