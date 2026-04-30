# கம்பைலர் மற்றும் ஃபிளாக்ஸ்
CC = gcc
CFLAGS = -Iinclude -Wall
LLVM_FLAGS = `llvm-config --cflags --ldflags --libs --system-libs`

# சோர்ஸ் கோப்புகள் (Source Files)
SRCS = src/main.c src/lexer.c src/parser.c src/codegen.c core/encoder.c core/decoder.c
CLI_SRC = src/tamizhi_cli.c

# அவுட்புட் பைனரிகள்
TARGET = tamizhi

# டிஃபால்ட் கமெண்ட்: 'make' அடிச்சா இது ரன் ஆகும்
all: setup $(TARGET)

# 1. தேவையான ஃபோல்டர்களை உருவாக்குதல்
setup:
	@mkdir -p storage
	@echo "[System] Storage folder initialized."

# 2. தமிழி மெயின் இன்ஜினை பில்ட் செய்தல்
$(TARGET): $(SRCS)
	@echo "[Building] Tamizhi Engine with DNA-VM Support..."
	@$(CC) $(CFLAGS) $(SRCS) $(LLVM_FLAGS) -o $(TARGET)
	@echo "[Success] Tamizhi is ready to use!"

# பழைய ஃபைல்களை நீக்க
clean:
	@echo "[Cleaning] Removing binaries and temporary files..."
	@rm -f $(TARGET)
	@rm -rf storage/*.dna
