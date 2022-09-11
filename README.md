
# Gakkota
Functional Language inspired by lisp

## About
Gekkota is a language inspired by lisp, it has a syntax somewhere in the common lisp universe but is adjusted to focus on more direct access for less formal consistency.

Where possible patterns are expected to stay continous, e.e. there are no positional arguments, there are binary counds like if/else but never cases where the first argument does something different than all the rest. all paradigms are indended to be consistent.

The system is modular and intended to be extended from the C source code level.

## Our first program example
After a while f development, here and there, the first working example to test the functional behaviour was as follows.

```
(
    .i 0
    .incr '(
        set .i (+ 1 i))
    .count '(
        (incr)
        (print "we have counted up to " i " with " value))

(count "alpha" "bravo" "charlie" "delta"))
```

which outputs the following:

```
we have counted up to 1 with alpha
we have counted up to 2 with bravo
we have counted up to 3 with charlie
we have counted up to 4 with delta
```

## Usablility ideas
The main goal is to have something that can be cooperative multiprocessing so the language is implemented engirely without recursion, the whole runtime is processed with a stack based structure, so that it can be paused and resumed in line with the needs of any other part of an application or library.

Functions are run by being the first element of a list indicated by parenthesis. For example `(print (+ 1 2 ))` will call the function `print` with the value of `3`. The balance between direct arguments or list arguments is balanced by setting values directly wihth the `.name <value>` notation, or set with `set .name <value>` or as a list such as `(print "hi" "there" "buddy")`. These paradigms are radically different and both necessary. In the above example `i` is set directly and then for the `count` function `value` is set as the funciton is run for each element in the list.
