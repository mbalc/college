CXXFLAGS=-c -I. -Wall -Wextra -O2 -std=c++17
CFLAGS=-c -Wall -Wextra -O2
CC=gcc
CXX=g++-7


all: $(patsubst test/%.cc, prog/%.e, $(wildcard test/*)) $(patsubst test/%.c, prog/%.e, $(wildcard test/*));
	echo $<

testarswlas: $(patsubst test/%.cc, out/%.err, $(wildcard test/*)) $(patsubst test/%.c, out/%.err, $(wildcard test/*));

make test:
	make && echo && echo && echo "~~~~~~~~~~~~~~~~~~~~~~~~~" && echo && echo && make testarswlas

debug: CXXFLAGS = -c -Wall -Wextra -std=c++17 -g
debug: CCFLAGS = -c -Wall -Wextra -g
debug: all

ndebug: CXXFLAGS += -DNDEBUG
ndebug: CCFLAGS += -DNDEBUG
ndebug: all


compile/%.o: %.cc;
	$(CXX) $(CXXFLAGS) $< -o $@

compile/%.o: test/%.cc;
	$(CXX) $(CXXFLAGS) $< -o $@
compile/%.o: test/%.c
	$(CC) $(CFLAGS) $< -o $@

prog/%.e: compile/%.o compile/sejf.o
	$(CXX) $^ -o $@

out/%.err: prog/%.e
	@echo ; echo "\\/\\/\\/\\/\\/\\/" ; echo "Executing test: ./$< 2> $@" ; ./$< ; if [ $$? -ne 0 ]; then echo ; echo "!  --  ERROR: Test $< gone bad!  --  !"; echo ; fi ; echo "/\\/\\/\\/\\/\\/\\" ; echo

clean clear:
	rm out/* prog/* compile/* ; mkdir out ; mkdir prog ; mkdir compile


.PHONY: all test
.PRECIOUS: prog/ compile/*
