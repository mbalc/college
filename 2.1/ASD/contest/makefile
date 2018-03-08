CXXFLAGS = -std=c++11 -O2 

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	@echo done

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

%: %.o;

clean: 
	rm *.o

