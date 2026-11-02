CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -I.

.PHONY: all test clean

all: lock

# Executable target
lock: main.o lock.o
	$(CC) $(CFLAGS) -o $@ $^

# Test target: builds and executes tests
test: test_lock
	./test_lock

test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) -o $@ $^

# Object file rules listing explicit header dependencies
main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c tests/test_lock.c -o tests/test_lock.o

clean:
	rm -f *.o tests/*.o lock test_lock