#include "../gekkota.h"

void init_basic_library(struct closure *closure){
    /** ---------- builtin --------=*/
    closure_add_value(closure, str("true"), new_result_value_obj(TRUE));
    closure_add_value(closure, str("false"), new_result_value_obj(FALSE));
    closure_add_value(closure, str("nil"), new_result_value_obj(NIL));
    closure_add_value(closure, str("error"), new_result_value_obj(ERROR));

    closure_add_function(closure, str("save-head"), new_save_head_operator(SAVE_HEAD));
    closure_add_function(closure, str("save-cell"), new_save_cell_operator(SAVE_CELL));
    closure_add_function(closure, str("save-value"), new_save_value_operator(SAVE_VALUE));
    closure_add_function(closure, str("save-count"), new_save_count_operator(SAVE_COUNT));

    /*----- logic -----*/
    closure_add_function(closure, str("if"), new_condition_operator(CONDITION));
    closure_add_function(closure, str("eq"), new_equal_operator(EQUAL));
    closure_add_function(closure, str("t"), new_boolean_operator(TRUTHY));
    /*
    closure_add_function(closure, str("and"), new_and_operator(CONDITION));
    closure_add_function(closure, str("or"), new_or_operator(CONDITION));
    closure_add_function(closure, str("not"), new_not_operator(CONDITION));
    closure_add_function(closure, str("isnil"), new_nil_operator(CONDITION));
    */

    /** ---------- arithemtic --------= */
    /* + */
    closure_add_function(closure, str("add"), new_arithmetic_operator(ADD));
    /* - */
    closure_add_function(closure, str("sub"), new_arithmetic_operator(SUBTRACT));
    /* / */
    closure_add_function(closure, str("div"), new_arithmetic_operator(DIVIDE));
    /* * */
    closure_add_function(closure, str("mul"), new_arithmetic_operator(MULTIPLY));

    closure_add_function(closure, str("lt"), new_arithmetic_operator(LESS_THAN));
    closure_add_function(closure, str("gt"), new_arithmetic_operator(GREATER_THAN));

    /* ---------- io ---------- */
    /* print */
    closure_add_function(closure, str("print"), new_print_operator(PRINT));
    /* open file */
    /* close file */
    /* open socket */
    /* close socket */
    /* read */
    /* read async */
    /* rename file */
    /* move file */

    /* ---------- sub ---------- */
    /* exec */
    /* wait */
    /* status */
    /* kill */
    /* deamonize */

    /* ---------- sqlite ---------- */

    /* ---------- webserver ---------- */

    /* ---------- string ---------- */
    /* concat */
    /* substr */
    /* to upper */
    /* to lower */
    /* trim */
    /* colors */

    /* ---------- regex ---------- */
    /* match */
    /* replace */
    /* get groups */

    /* -------- arrays --------= */
    /* split */
    /* index */
    /* concat */

    /* -------- objects --------= */
    /* clone creation */
    /* property access */

    /* --------= logic --------= */
    /* else if */
    /* map */ 
    /* reduce */ 
    /* for */ 
}
