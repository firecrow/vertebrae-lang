
# Gekkota
Iteration Based Programming Language

## About
Gekkota is a language inspired by lisp, it has a syntax somewhere in the common lisp universe but is adjusted to focus on more direct access for less formal consistency.

Where possible patterns are expected to stay continous, e.e. there are no positional arguments, there are binary counds like if/else but never cases where the first argument does something different than all the rest. all paradigms are indended to be consistent.

The system is modular and intended to be extended from the C source code level.

## Usablility ideas
The main goal is to have something that can be cooperative multiprocessing so the language is implemented engirely without recursion, the whole runtime is processed with a stack based structure, so that it can be paused and resumed in line with the needs of any other part of an application or library.

Functions are run by being the first element of a list indicated by parenthesis. For example `(print (+ 1 2 ))` will call the function `print` with the value of `3`. The balance between direct arguments or list arguments is balanced by setting values directly wihth the `.name <value>` notation, or set with `set .name <value>` or as a list such as `(print "hi" "there" "buddy")`. These paradigms are radically different and both necessary. In the above example `i` is set directly and then for the `count` function `value` is set as the funciton is run for each element in the list.

# Most influential objects

## Cell
A cell is a single unit of value, this value can be a static value like a number or string, or it can be a symbol pointing to another value or an operator.

## Operator
The `operator` objects are the way behavior is excecuted, they behave much like the functions of bodies and are assigned to a `head` object when the `head` object is  going to recieves a series of cells.

## Head
`head` objects are records that join a series of cells to an operator.


# Layout of the code

### Root dirs 
*src* holds all of the langauge code and *test* holds all of the tests, for the rest of this section it is assumed *src* is the root directory.

### core
This directory handles all central actions like instantiating `head` objects for when a new list of cells is being sent to a `head` object. head objects are essentially like functions they recieve cells and do operations on them.

### run
The run directory holds a minimum router for the running through the cells, some operators (covered below) also run through the cells for operations such as if/else navigation which is not handled in this folder.



# Diagrams

### Cell/branch flow

This is the flow of how functions (represented by the head of the segment) are follow by cells which make up the control flow of the language.

![Gekkota cell flow](docs/gekkota-cell-flow.png)

This code would look like:

       print <- "Its", 
           + 10 4,
           "Degrees Outside"

And it would output `It's 14 Degrees Outside"`

### Head/Operator flow 

As each segment head is encourtered it is assigned an operator, these do everything from print content to a stream, do arithmetic based on cell values, or set the navigation of if/else conditions. Operators do most of the work in the language.

![Gekkota Head Flow](docs/gekkota-head-value-flow.png)
