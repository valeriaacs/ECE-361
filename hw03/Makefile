CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I.

# Targets
ALL = ptr.o test/test_ptr

all: $(ALL)

# Build object files
ptr.o: ptr.c ptr.h
	$(CC) $(CFLAGS) -c ptr.c -o ptr.o

tests/test_ptr.o: test/test_ptr.c ptr.h
	$(CC) $(CFLAGS) -c test/test_ptr.c -o test/test_ptr.o

# Build binary
tests/test_ptr: test/test_ptr.o ptr.o
	$(CC) $(CFLAGS) test/test_ptr.o ptr.o -o test/test_ptr -lm


# Run tests target
test: test/test_ptr
	./test/test_ptr

# Clean built files
clean:
	rm -f ptr.o test/test_ptr.o tests/test_ptr

.PHONY: all test clean
