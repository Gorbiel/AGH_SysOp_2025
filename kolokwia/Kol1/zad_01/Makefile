CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared
LIB_DIR = ./lib
BIN_DIR = ./bin


all: prepare_dirs $(LIB_DIR)/bibl1.so $(BIN_DIR)/zad1


prepare_dirs:
	mkdir -p $(LIB_DIR) $(BIN_DIR)


$(LIB_DIR)/bibl1.so: bibl1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


$(BIN_DIR)/zad1: zad1.c
	$(CC) -o $@ $^ -ldl


test: all
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH $(BIN_DIR)/zad1