
CC = gcc
# LLVM Flags-ஐ ஒரு வேரியபிளா வெச்சுக்கிட்டா பாக்குறதுக்கு கிளீனா இருக்கும்
LLVM_FLAGS = `llvm-config --cflags --ldflags --libs core`
CFLAGS = -Iinclude -Wall

# டிஃபால்ட்டா ரெண்டு ஃபைலையும் பில்ட் பண்ணும்
all: tamizhi_core tamizhi

# 1. தமிழி கோர் (The Compiler Backend)
tamizhi_core: src/main.c src/lexer.c src/parser.c src/codegen.c
	@echo "[Building] Tamizhi Core..."
	@$(CC) $(CFLAGS) src/main.c src/lexer.c src/parser.c src/codegen.c $(LLVM_FLAGS) -o tamizhi_core

# 2. தமிழி CLI (The Frontend Tool)
tamizhi: src/tamizhi_cli.c
	@echo "[Building] Tamizhi CLI..."
	@$(CC) src/tamizhi_cli.c -o tamizhi

# பழைய பைனரி ஃபைல்களை நீக்க
clean:
	@echo "[Cleaning] Removing binaries..."
	@rm -f tamizhi_core tamizhi

