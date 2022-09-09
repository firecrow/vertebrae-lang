#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"

struct closure *init_basic_library(struct closure *closure){
    /** ========== builtin =========*/
    closure_add_function(closure, str("cond"), new_condition_operator(CONDITION));
    closure_add_value(closure, str("true"), new_result_value_obj(TRUE));
    closure_add_value(closure, str("false"), new_result_value_obj(FALSE));
    closure_add_value(closure, str("nil"), new_result_value_obj(NIL));
    closure_add_value(closure, str("error"), new_result_value_obj(ERROR));

    /** ========== arithemtic ========= */
    /* + */
    closure_add_function(closure, str("+"), new_arithmetic_operator(ADD));
    /* - */
    closure_add_function(closure, str("-"), new_arithmetic_operator(SUBTRACT));
    /* / */
    closure_add_function(closure, str("/"), new_arithmetic_operator(DIVIDE));
    /* * */
    closure_add_function(closure, str("*"), new_arithmetic_operator(MULTIPLY));

    /* ========== io ========== */
    /* print */
    closure_add_function(closure, str("print"), new_print_operator(PRINT));

    /* ========== string ========== */
    /* concat */
    /* substr */
    /* to upper */
    /* to lower */
    /* trim */

    /* ======== arrays ========= */
    /* split */
    /* index */
    /* concat */

    /* ======== objects ========= */
    /* creation */
    /* property access */

    /* ========= logic ========= */
    /* else if */
    /* map */ 
    /* reduce */ 
    /* for */ 
}
