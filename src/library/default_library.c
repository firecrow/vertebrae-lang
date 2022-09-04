#include "../external.h"
#include "../ssimple.h"
#include "../core/core.h"
#include "../types/types.h"
#include "../operators/operator.h"

struct closure *init_basic_library(struct closure *closure){
    /** ========== variabls =========*/
    closure_add_function(closure, str("let"), new_definition_operator(DEFINITION));
    closure_add_function(closure, str("closure"), new_closure_operator(CLOSURE));

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
