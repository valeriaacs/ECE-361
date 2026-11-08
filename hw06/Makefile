CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
LIBS = -lm

OBJS = main.o sensor.o panel.o scenario.o iom361_r4.o
TEST_OBJS = tests/test_hw06.o sensor.o panel.o scenario.o iom361_r4.o

all: replay

replay: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

test_hw06: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIBS)

test: replay test_hw06
	./test_hw06
	./scenarios/regress.sh

# Explicit object rules with header dependencies
main.o: main.c iom361_r4.h sensor.h panel.h scenario.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c tests/test_hw06.c -o tests/test_hw06.o

clean:
	rm -f *.o tests/*.o replay test_hw06 tmp_*.trace

.PHONY: all test clean