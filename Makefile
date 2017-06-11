
all:
    $(CXX) -pedantic -std=c++11 -Wall -Wextra -g hitoban.cpp       -o hitoban.o

test:
    ./hitoban || exit 1