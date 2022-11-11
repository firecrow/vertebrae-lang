enum OPERATOR_TYPE {
    CUSTOM = 0,
    DEFINE,
    SET,
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
    SAVE_HEAD,
    SAVE_CELL,
    SAVE_VALUE,
    SAVE_COUNT,
    GREATER_THAN,
    LESS_THAN
};

enum gka_op_lifecycle {
    GKA_OP_NOT_STARTED = 0,
    GKA_OP_STARTED,
    GKA_OP_CLOSING,
};

enum SL_BRANCH_TYPE {
    SL_CONTINUE =0,
    SL_BREAK
};

typedef bool (operator_handle_func)(struct operator_ifc *_op, struct crw_state *ctx);

struct operator_ifc {
    enum OPERATOR_TYPE type;
    struct operator_ifc *(*new)(enum OPERATOR_TYPE type);
    operator_handle_func *handle;
    operator_handle_func *close;
    enum gka_op_lifecycle lifecycle;
};

void default_next(struct crw_state *ctx);

struct operator_ifc * new_def_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_set_operator(enum OPERATOR_TYPE type);
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
struct operator_ifc * new_save_head_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_save_cell_operator(enum OPERATOR_TYPE type);
struct operator_ifc * new_save_value_operator(enum OPERATOR_TYPE type);

struct operator_ifc * new_save_count_operator(enum OPERATOR_TYPE type);
