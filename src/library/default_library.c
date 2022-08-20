struct closure *init_basic_library(struct closure *closure){
    /** ========== variabls ========= */
    closure_add_function(closure, "var", new_definition_operator(VARIABLE);
    closure_add_function(closure, "func", new_definition_operator(FUNCTION);

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
