all:
    $(CXX) -pedantic -std=c++11 -Wall -Wextra -g hitoban.cpp       -o hitoban.o
    //$(CXX) -pedantic -std=c++11 -Wall -Wextra -g htb_functions.cpp -o htb_functions.o
    //$(CXX) -pedantic -std=c++11 -Wall -Wextra -g htb_stdlib.cpp    -o htb_stdlib.o
    //$(CXX) -o hitoban hitoban.o htb_functions.o htb_stdlib.o  


test:
    ./hitoban || exit 1