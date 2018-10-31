# poll_echo_srv Makefile

CC = gcc
DEBUG = -g
CFLAGS = -Wall $(DEBUG) -I../include

UTIL_SRC = \
    ../include/utils.c \
    ../include/log.c

SRV_TARTET = poll_echo_srv
SRV_SRC = poll_echo_srv.c $(UTIL_SRC)

TARGET = \
    $(SRV_TARTET) \

all: $(TARGET)

$(SRV_TARTET): $(SRV_SRC)
    $(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
    rm $(TARGET)