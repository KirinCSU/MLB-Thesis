CXX = g++
CXXFLAGS = -std=c++17 -w
INCLUDES = -Iinclude -I../Data/include -I../Research/include

APP_SRC = $(wildcard apps/*.cpp)
DATA_SRC = $(wildcard ../Data/src/*.cpp)
RESEARCH_SRC = $(wildcard ../Research/src/*.cpp)
COMMON_SRC = $(DATA_SRC) $(RESEARCH_SRC)

TARGETS = $(patsubst apps/%.cpp, bin/%, $(APP_SRC))

all: $(TARGETS)

bin/%: apps/%.cpp $(COMMON_SRC)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f bin/*
