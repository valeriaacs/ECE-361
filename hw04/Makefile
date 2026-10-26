CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
INCLUDES = -I.

# Target executables
TEST_RUNNER = test_tally_runner
EVENTS_PROG = events

# Object files
OBJS = tally.o

all: $(TEST_RUNNER) $(EVENTS_PROG)

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c -o tally.o

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c -o events.o

$(TEST_RUNNER): tests/test_tally.c tally.o
	$(CC) $(CFLAGS) $(INCLUDES) tests/test_tally.c tally.o -o $(TEST_RUNNER)

$(EVENTS_PROG): events.o tally.o
	$(CC) $(CFLAGS) events.o tally.o -o $(EVENTS_PROG)

test: $(TEST_RUNNER)
	./$(TEST_RUNNER)

clean:
	rm -f *.o $(TEST_RUNNER) $(EVENTS_PROG)

.PHONY: all test clean