CXXFLAGS = -std=c++11 -static -O2 -s -lm -Wall -Wextra -pedantic
CXXDEBUG = -std=c++11 -g

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	@echo done

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

%.g: %.cpp
	$(CXX) $(CXXDEBUG) -o $@ $<

%: %.o;

clean: 
	rm *.o

