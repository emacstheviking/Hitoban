# Hitoban

[![Build Status](https://travis-ci.org/Loodoor/Hitoban.svg?branch=master)](https://travis-ci.org/Loodoor/Hitoban)

# Introduction

Hitoban is an interpreted programming language, in C++ (which respects the C++11 norm). It is a lisp-like (no please don't quit us now !) :smiley:

For the small history, the language is named like this because a "millipede" is called "Hitoban" in Japanese, 
and all those brackets make a Hitoban code looks like a millipede. :)

We chosed a lisp-like syntax because it was really easy to tokenize and to parse, and so to execute. The main purpose of this language is to offer something reliable, with 
the less bugs possible (goal is 0), and very easy to use in any C++ project. So you understand why the grammar is so basic and can seems to be redundant. :wink:

Original idea from @ofan (see https://gist.github.com/ofan/721464)

[![Hitoban demonstration](http://img.youtube.com/vi/IdgDsYX4yiE/0.jpg)](http://www.youtube.com/watch?v=IdgDsYX4yiE)

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

## Using Hitoban in a C++ project

Download the [project](https://github.com/Loodoor/Hitoban/archive/master.zip) and extract it in a directory called `hitoban` (for example) in your project.

All you need to do is to include `FOLDER WHERE YOU DOWNLOADED HTB/hitoban.hpp`, and then to execute Hitoban code, follow these steps :

```cpp
// we strongly advise you to create only one Hitoban environment and to keep it
htb::environment env = htb::init_environment();
htb::cell value = htb::run_string("CODE HERE", env);
// if you want to print the value the Hitoban interpreter returned you :
std::cout << htb::to_string(value) << htb::endl;
```

## Using Hitoban in Notepad++

Download the [lang.xml](lang.xml) file. Then open Notepad++, go in `Language` > `Define your language` > `Import` (button) and select the lang.xml file you downloaded before. Then exit Notepad++ and restart it. You can now type Hitoban code and have it coloured ! :smile:

# Building

## Building the Hitoban interpreter

`cd [Hitoban sources directory] && make`

To build and launch the tests : `make tests`

<!-- # What's new

// something new ? tell us -->

# Documentation

See the [specification](specification.md)

You can also see the syntax here : https://gist.github.com/Loodoor/f0a8cd0baf38449e43e787f91371a5c2

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