CXXFLAGS=-c -Isrc -Wall -Wextra -O2 -std=c++17
CFLAGS=-c -Isrc -Wall -Wextra -O2
CC=gcc
CXX=g++
DEPS=src/dict.h src/dictglobal.h

all: $(patsubst test/%.cc, out/%, $(wildcard test/*)) $(patsubst test/%.c, out/%, $(wildcard test/*));

compile/dict.o: src/dict.cc
	$(CXX) -Isrc $(CXXFLAGS) $< -o $@

compile/dictglobal.o: src/dictglobal.cc
	$(CXX) -Isrc $(CXXFLAGS) $< -o $@

compile/%.o: test/%.cc;
	$(CXX) $(CXXFLAGS) $< -o $@
compile/%.o: test/%.c
	$(CC) $(CFLAGS) $< -o $@

out/%: compile/%.o compile/dict.o compile/dictglobal.o
	g++ $^ -o $@

clean clear:
	rm out/* compile/*

.PHONY: all
.PRECIOUS: out/* compile/*
