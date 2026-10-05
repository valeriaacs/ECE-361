CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I.

# Source files and object files
SRCS = bits.c status.c
OBJS = $(SRCS:.c=.o)

#Test executable
TEST_TARGET = tests/test_bits
TEST_SRC = tests/test_bits.c

.PHONY: all test clean

#Default rule
all: $(OBJS)

#Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#Build and run tests
test: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

#Remove build artifacts
clean:
	rm -f *.o $(TEST_TARGET)