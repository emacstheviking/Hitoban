#!/bin/bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

make || exit 1
make tests || exit 1

#tests:
#	./bin/Debug/hitoban tests || exit 1
#	./bin/Release/hitoban tests || exit 1
