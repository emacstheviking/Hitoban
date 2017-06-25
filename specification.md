# Official documentation for the programming language Hitoban

## Create a variable

### Simple variable

`(def varname value)`

### Const variable

`(const varname value)`

### Naming

You can name a variable following this rule :

`^[#@$':_!?\\-\\w]+`

So `@#$:hello-_world` is a valid variable name

## Language defined constants

`nil` => None (in Python)

`false`, `true`

## Operators

All the available operators :

`+ - * / % <= >= != < > = & | ^`

`%` is the modulo

`&` an `and`

`|` an or

`^` pow

## Types

### Numbers

Defined as follow :

`^((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?`

So, `-5.e+17` is a number

### Strings

Defined as follow :

`^['"][^'"]+['"]`

As strange as it is, `"test'` is a valid string

### Lists

`(list ...)`

With `...` every type of variable you want, each element separated from the others by a space

### Dictionaries

`(dict (list key value) ...)`

With `...` others pairs of `key` => `value`

Key must be a string here

Alternative syntax :

`(dict (:key value) ...)`

Key is just a word, so typing : `(dict (:test_hello-foo@ my-element))` is correct

### Lambdas

`(lambda (args...) (body))`

A lambda can take 0 to infinite number of arguments.

If either there is no argument to give or no body, the bloc must be present, even if it is empty :

`(lambda () ())`

The value of the last bloc evaluated in the body of the lambda is the returned value when calling it :

```(lambda (x) (begin
    (set! x (+ x 3))
    x
))```

will return x+3

## Comments

Only single line comments using `; your comment here`

## Hash-syntax (getting element in a list/dictionary/string)

`(#key my-dict)` => return the element at `key` in the dictionary `my-dict`

`(#5 my-list)` => return the 6th element in the list `my-list`

`(#3 my-string)` => return the 4th character in the string `my-string`

## Language defined procedures

`(quote thing)` => return thing, without evaluating it

`(begin ...)` with `...` an infinite number of blocs of code to evaluate. Return the value of the last bloc evaluated

`(ns "name" ...)` => same as `begin` but create a sub-environment inheriting from its upper environment, named `name`

You can access elements (or add, it is the same to Hitoban) just by using `(ns "name" your-code-here...)`

`(i-ns "name" ...)` => same as `ns` but does not inherit from upper environment, only basics procedures are included in the *isolated namespace* created

`(require "file.htb")` => include the Hitoban code in `file.htb` in the current environment

`(require (list "file1.htb" "file2.htb"))` => include the Hitoban code, in all the specified files, in the current environment

`(require (dict (:maths "lib/maths.htb")))` => include the maths lib in a namespace `maths`

## Conditions

`(if cond then alt)` `alt` is optional

`cond` is an expression returning a boolean statement

`then` is a bloc of code

## Calling a procedure

`(procedure-name arguments...)`

## Standard lib

`append` => takes a list as the first argument, then all the thing you want to add in the list. Return a new list

`car` => as `car` in lisp

`cdr` => as `cdr` in lisp

`cons` => as `cons` in lisp

`length` => returns the length of an object (either a list, a dictionary or a string)

`list` => takes 0 to infinite number of argument. Return a list containing all the elements provided (copied)

`null?` => is the element empty ? A string, number, procedure and a dictionary is always seen as empty. Useful to check if a list has elements in it.

`nth` => takes a number/string and a list/dictionary/string, and return the n th element in it (for dictionaries, return the element addressed by the given key)

`cond` => example :

```
(cond (
        (< x 10)
        (print "x < 10")
    )
    (
        nil
        (print "all the tests failed")
    )
)
```

`values` => returns a list containing the values of a given dictionary (copying)

`keys` => returns a list containing the keys of a given dictionary (copying)

`print` => print all the given arguments, separated by a single space. Ended by OS line ending

`prin1` => print a single string, without adding spaces or '\n' / '\r' / '\r\n'

`input` => can take an argument. If an argument is given (string only), prints it. Then asks the user to write something

`getc` => asks the user a single character

`system` => takes a string and execute it as a shell command

`str-eq` => comparing 2 or more string

`str-cat` => concatenating 2 or more strings

`str-reverse` => returning a new string which is the string given in argument, but reversed

`str-format` => takes a string as the first argument, then a bunch of arguments. Following this specification : https://github.com/ryjen/format

`typeof` => returns the type (string) of the given argument

`random` => takes 0 to 2 arguments. If 0 arguments are given, returning a random number between 0 and 1 (floating point number). If 1 argument is given, returns a random number between 0 and the number given (integer). If 2 arguments are given, returns a random number between the two numbers (each are included)