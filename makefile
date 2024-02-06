CC = clang
CFLAGS = -Wall -Wextra -std=c11

SRC_FILE = lights_out.c
BIN_DIR = bin

BIN_TARGET = $(BIN_DIR)/lights_out

.PHONY: all build test clean

all: build

build: 
	mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_FILE) -o $(BIN_TARGET)	

test:

clean:
	$(RM) -r $(BIN_DIR)	


