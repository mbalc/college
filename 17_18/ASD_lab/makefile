CXXFLAGS = -std=c++11 -static -O2 -s -lm

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	@echo done

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

%: %.o;

clean: 
	rm *.o

