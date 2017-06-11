
all:
	$(CXX) -pthread -std=c++11 -Wall -Wextra hitoban.cpp -o hitoban

test:
	./hitoban || exit 1