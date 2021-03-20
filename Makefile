# shell

CXX       := clang++
CXXFLAGS  := -std=c++17 -Isrc/
SRC       := $(wildcard src/*.cc) $(wildcard src/*/*.cc)
LIBS      := -lreadline
NAME      := shell

.PHONY: all

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(NAME)

debug:
	$(CXX) $(CXXFLAGS) -g -D_DEBUG $(SRC) $(LIBS) -o $(NAME)

