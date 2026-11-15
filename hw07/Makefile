CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -pedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

all: balance

balance: balance.o istack.o
	$(CC) $(CFLAGS) -o balance balance.o istack.o

tests/test_istack: tests/test_istack.o istack.o
	$(CC) $(CFLAGS) -o tests/test_istack tests/test_istack.o istack.o

balance.o: balance.c istack.h
	$(CC) $(CFLAGS) -c balance.c -o balance.o

istack.o: istack.c istack.h
	$(CC) $(CFLAGS) -c istack.c -o istack.o

tests/test_istack.o: tests/test_istack.c istack.h
	$(CC) $(CFLAGS) -c tests/test_istack.c -o tests/test_istack.o

test: balance tests/test_istack
	./tests/test_istack
	./regress.sh

memcheck: clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANFLAGS)" test

clean:
	rm -f *.o tests/*.o balance tests/test_istack