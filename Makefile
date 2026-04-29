CC = gcc
# LLVM config moolama flags-a edukkuroam
LLVM_FLAGS = `llvm-config --cflags --ldflags --libs core`
CFLAGS = -Iinclude -Wall
# Unga src files list
SRC = src/main.c src/lexer.c src/parser.c src/codegen.c
OBJ = tamizhi_core

all:
	$(CC) $(CFLAGS) $(SRC) $(LLVM_FLAGS) -o $(OBJ)

clean:
	rm -f $(OBJ)
