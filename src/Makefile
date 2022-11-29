#### debug
CXXFLAGS := -O1 -g -fsanitize=address -fno-omit-frame-pointer -Wall -std=c++17
LDFLAGS = -fsanitize=address

#### fast
# CXXFLAGS := -std=c++17

all: pentago

pentago: board.o position.o minimax.o
	$(CXX) $(LDFLAGS) -o $@ $^