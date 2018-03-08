CC = gcc
CFLAGS = -O2 -std=c99 -Wall -Wextra -D NDEBUG
GDBFLAGS = -g -O0 -std=c99 -Wall -Wextra
TARGET = solution
DEPENDENCIES = tree.o
GDBDEPENDENCIES = tree.og

$(TARGET): $(TARGET).c $(DEPENDENCIES)
	$(CC) $(CFLAGS) -o $@ $^ 

debug: $(TARGET).dbg 


%.dbg: %.c $(GDBDEPENDENCIES)
	$(CC) $(GDBFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.og: %.c
	$(CC) $(GDBFLAGS) -c $< -o $@



.PHONY: clean debug

clean: 
	rm tree.o tree.og
