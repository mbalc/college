CXXFLAGS=-c -Isrc -Wall -Wextra -O2 -std=c++17
CFLAGS=-c -Isrc -Wall -Wextra -O2
CC=gcc
CXX=g++
DEPS=src/dict.h src/dictglobal.h


all: $(patsubst test/%.cc, prog/%.e, $(wildcard test/*)) $(patsubst test/%.c, prog/%.e, $(wildcard test/*));

test: $(patsubst test/%.cc, out/%.err, $(wildcard test/*)) $(patsubst test/%.c, out/%.err, $(wildcard test/*));


debug: CXXFLAGS = -c -Isrc -Wall -Wextra -std=c++17 -g
debug: CCFLAGS = -c -Isrc -Wall -Wextra -g
debug: all

ndebug: CXXFLAGS += -DNDEBUG
ndebug: CCFLAGS += -DNDEBUG
ndebug: all


compile/%.o: src/%.cc;
	$(CXX) $(CXXFLAGS) $< -o $@

compile/%.o: test/%.cc;
	$(CXX) $(CXXFLAGS) $< -o $@
compile/%.o: test/%.c
	$(CC) $(CFLAGS) $< -o $@

prog/%.e: compile/%.o compile/dict.o compile/dictglobal.o  
	g++ $^ -o $@

out/%.err: prog/%.e
	@echo "./$< 2> $@" ; ./$< > out/$@ 2> $@ ; if [ $$? -ne 0 ]; then echo ;echo "!  --  ERROR: Test $< gone bad!  --  !"; echo ; fi

clean clear:
	rm out/* prog/* compile/* ; mkdir out ; mkdir prog ; mkdir compile


.PHONY: all test
.PRECIOUS: prog/ compile/*
