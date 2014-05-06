TARGETS=D4
SRC=$(wildcard *.cpp)

CXXFLAGS+=-std=gnu++0x -g -Wall

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)

$(TARGETS): $(SRC:.cpp=.o)
	$(CXX) $^ $(CXXFLAGS) -o $@

%.o: %.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@
