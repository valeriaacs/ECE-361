CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c
OBJS = $(SRCS:.c=.o)
TARGET = logger

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@echo "No tests configured yet."

clean:
	rm -f $(TARGET) *.o

.PHONY: all test clean