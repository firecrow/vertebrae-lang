#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"

struct closure *init_basic_library(struct closure *closure){
    /** ---------- builtin --------=*/
    closure_add_value(closure, str("true"), new_result_value_obj(TRUE));
    closure_add_value(closure, str("false"), new_result_value_obj(FALSE));
    closure_add_value(closure, str("nil"), new_result_value_obj(NIL));
    closure_add_value(closure, str("error"), new_result_value_obj(ERROR));


    /*----- logic -----*/
    closure_add_function(closure, str("if"), new_condition_operator(CONDITION));
    closure_add_function(closure, str("="), new_equal_operator(CONDITION));
    /*
    closure_add_function(closure, str("and"), new_and_operator(CONDITION));
    closure_add_function(closure, str("or"), new_or_operator(CONDITION));
    closure_add_function(closure, str("="), new_equal_operator(CONDITION));
    closure_add_function(closure, str("!"), new_not_operator(CONDITION));
    closure_add_function(closure, str("<"), new_less_than_operator(CONDITION));
    closure_add_function(closure, str(">"), new_greater_than_operator(CONDITION));
    closure_add_function(closure, str("?"), new_nil_operator(CONDITION));
    */

    /** ---------- arithemtic --------= */
    /* + */
    closure_add_function(closure, str("+"), new_arithmetic_operator(ADD));
    /* - */
    closure_add_function(closure, str("-"), new_arithmetic_operator(SUBTRACT));
    /* / */
    closure_add_function(closure, str("/"), new_arithmetic_operator(DIVIDE));
    /* * */
    closure_add_function(closure, str("*"), new_arithmetic_operator(MULTIPLY));

    /* ---------- io ---------- */
    /* print */
    closure_add_function(closure, str("print"), new_print_operator(PRINT));

    /* ---------- string ---------- */
    /* concat */
    /* substr */
    /* to upper */
    /* to lower */
    /* trim */

    /* -------- arrays --------= */
    /* split */
    /* index */
    /* concat */

    /* -------- objects --------= */
    /* creation */
    /* property access */

    /* --------= logic --------= */
    /* else if */
    /* map */ 
    /* reduce */ 
    /* for */ 
}
