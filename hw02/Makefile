CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

SRCS = main.c reading.c stats.c histogram.c
OBJS = $(SRCS:.c=.o)
TARGET = readings

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_stats: tests/test_stats.c stats.o reading.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(TARGET) test_stats
	@echo "--- Running Unit Tests ---"
	./test_stats
	@echo "--- Running Regression Tests ---"
	bash regress.sh

clean:
	rm -f $(TARGET) test_stats *.o