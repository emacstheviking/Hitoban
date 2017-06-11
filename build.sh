#!/bin/bash

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

make || exit 1