# Hitoban

[![Build Status](https://travis-ci.org/Loodoor/Hitoban.svg?branch=master)](https://travis-ci.org/Loodoor/Hitoban)

# Introduction

Hitoban is an interpreted programming language, in C++ (which respects the C++11 norm). It is a lisp-like (no please don't quit us now !) :smiley:

For the small history, the language is named like this because a "millipede" is called "Hitoban" in Japanese, 
and all those brackets make a Hitoban code looks like a millipede. :)

We chosed a lisp-like syntax because it was really easy to tokenize and to parse, and so to execute. The main purpose of this language is to offer something reliable, with 
the less bugs possible (goal is 0), and very easy to use in any C++ project. So you understand why the grammar is so basic and can seems to be redundant. :wink:

# Goals

* no bugs
* 0 dependencies
* do not stop on exception, or at least try
* no warning when compiling with `g++ -Wall -std=c++11 -pedantic -Wextra -g`

# Using Hitoban

## How to execute a Hitoban file

`./hitoban [file path here] [args]`

Arguments, which must be separated by spaces, are given to the script into a `const list` called `ARGS`.

## Using the Hitoban shell

`./hitoban -w` => turns Hitoban in strict mode (should stop after getting an exception). Raises a `std::runtime_error`

`./hitoban -t` => turns Hitoban in tracking mode : it will print the type and content of each variable evaluated

`./hitoban -v` => prints the version of the language

`./hitoban -h` => prints the help message

`./hitoban` => start a simple *repl* (read eval print loop)

Those options can be cumulated : `-t`, `-w` with `file [args...]`.

Example of command using both the tracking mode and the strict mode while executing an Hitoban code in a file `file.htb`, giving `5` as an argument of the script :

`./hitoban -t -w file.htb 5`

Something like this will show up :

```
Hitoban 1.0.26.155 (last build on 16/06/2017) [status Alpha]
Type "help" for more information.
> 
```

# Building

## Building the Hitoban interpreter

You just need to compile the code as follow, no dependencies are needed.

```
$ g++ -Wall -std=c++11 -pedantic -Wextra -g  -c hitoban.cpp       -o obj\hitoban.o
$ g++ -Wall -std=c++11 -pedantic -Wextra -g  -c htb_functions.cpp -o obj\htb_functions.o
$ g++ -Wall -std=c++11 -pedantic -Wextra -g  -c htb_stdlib.cpp    -o obj\htb_stdlib.o
$ g++ -o bin\hitoban obj\hitoban.o obj\htb_functions.o obj\htb_stdlib.o  
```

(Using the flags `-fexpensive-optimizations -O3` can reduce drastically the size of the executable file produced)

You can also use the makefile : `cd [Hitoban sources directory] && make`

To launch the tests while building : `make tests`

# What's new

// something new ? tell us

# Documentation

// link to documentation

For the moment, take a look at the syntax here : https://gist.github.com/Loodoor/f0a8cd0baf38449e43e787f91371a5c2

# Testing

Call the Hitoban interpreter with the option `tests`, as following :

`./hitoban tests`

Example output :

```
Hitoban 1.0.26.155 (last build on 16/06/2017) [status Alpha]
hello world
hello hello 6
5
hello 6
total tests 77, total failures 0
```

# Proposal for enhancement

If you have any idea to improve the language, please open an issue and tag it as `PFP` (Proposal For Enhancement). We will try to review as soon as possible, and if
your PFP is accepted (and developed by the way), it will be published under https://kyatchioru.tk/Hitoban/PFP

# Copyright and license

The project is currently developed by Loodoor, and is under MIT license, so feel free to use it in any commercial project or whatever.