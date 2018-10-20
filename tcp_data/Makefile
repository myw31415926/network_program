# tcp_echo Makefile

CC = gcc
DEBUG = -g
CFLAGS = -Wall $(DEBUG) -I../include

UTIL_SRC = \
	../include/utils.c \
	../include/log.c

SRV_TARTET = data_srv
SRV_SRC = data_srv.c $(UTIL_SRC)

CLI_TARTET = data_cli
CLI_SRC = data_cli.c $(UTIL_SRC)

TARGET = \
	$(SRV_TARTET) \
	$(CLI_TARTET) \

all: $(TARGET)

$(SRV_TARTET): $(SRV_SRC)
	$(CC) $(CFLAGS) $^ -o $@

$(CLI_TARTET): $(CLI_SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm $(TARGET)