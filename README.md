# Hitoban

// need to setup build environment (travis or alike)

# Introduction

Hitoban is an interpreted programming language, in C++ (which respects the std11 norm). It is a lisp-like (noo please don't quit us now !) :)

For the small history, the language is named like this because a "millipede" is called "Hitoban" in Japanese, 
and all those brackets make a Hitoban code looks like a millipede. :)

We choose a lisp-like syntax because it was really easy to tokenize and to parse, an so to execute. The main purpose of this language is to offer something reliable, with 
the less bugs possible (goal is 0), and very easy to use in any C++ project. So you understand why the grammar is so basic and can seems to be redundant. ;)

# Goals

* no bugs
* do not stop on exception, or at least try
* no warning when compiling with `g++.exe -Wall -std=c++14 -pedantic -Wextra -g`

# Using Hitoban

// add how to use Hitoban in a shell

// add how to use the Hitoban shell

# Building

// add building instructions

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
hello world
5
total tests 64, total failures 0
```

# Proposal for enhancement

// give a link to add an idea or suggest to create an issue

# Copyright and license

The project is currently developed by Loodoor, and is under MIT license, so feel free to use it in any commercial project or whatever.