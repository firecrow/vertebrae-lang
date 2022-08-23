#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"

struct closure *init_basic_library(struct closure *closure){
    /** ========== variabls ========= 
    closure_add_function(closure, str("var"), new_definition_operator(VARIABLE));
    closure_add_function(closure, str("func"), new_definition_operator(FUNCTION));
    */

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
