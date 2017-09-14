#!/bin/bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

echo "make all"
make || exit 1
echo "------------------------------------"
echo "make tests"
make tests || exit 1
