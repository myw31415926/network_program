# utils Makefile

CC = gcc
DEBUG = -g
CFLAGS = -Wall $(DEBUG) -I./

UTIL_SRC = \
	./utils_test.c \
	./utils.c \
	./log.c

TARGET = utils_test

all: $(TARGET)

$(TARGET): $(UTIL_SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm $(TARGET)