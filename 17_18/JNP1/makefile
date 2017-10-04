CXXFLAGS = -Wall -Wextra -O2 -std=c++17

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	@echo done

%: %.cpp
	@echo try target $@.o

clean: 
	rm *.o

