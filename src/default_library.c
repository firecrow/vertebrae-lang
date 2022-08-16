struct closure *init_basic_library(struct closure *closure){
    /** ========== arithemtic ========= */
    /* + */
    closur_add_function(closure, "+", new_arithmatic_operator(ADD));
    /* - */
    closur_add_function(closure, "-", new_arithmatic_operator(SUBTRACT));
    /* / */
    closur_add_function(closure, "/", new_arithmatic_operator(DIVIDE));
    /* * */
    closur_add_function(closure, "*", new_arithmatic_operator(MULTIPLY));

    /* ========== string ========== */
    /* print */
    closur_add_function(closure, "print", new_print_operator();
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
