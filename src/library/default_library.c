#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"

struct closure *init_basic_library(struct closure *closure){
    /** ========== condition =========*/
    closure_add_function(closure, str("cond"), new_condition_operator(CONDITION));

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
