CXX = g++
CXXFLAGS = -std=c++17 -w
INCLUDES = -Iinclude -I../Base/include -I../Data/include

APP_SRC = $(wildcard apps/*.cpp)
BASE_SRC = $(wildcard ../Base/src/*.cpp)
DATA_SRC = $(wildcard ../Data/src/*.cpp)
LOCAL_SRC = $(wildcard src/*.cpp)
COMMON_SRC = $(BASE_SRC) $(DATA_SRC) $(LOCAL_SRC)

TARGETS = $(patsubst apps/%.cpp, bin/%, $(APP_SRC))

all: $(TARGETS)

bin/%: apps/%.cpp $(COMMON_SRC)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f bin/*
