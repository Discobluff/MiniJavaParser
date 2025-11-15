CC = gcc
CFLAGS = -Wall -Wextra -Wno-missing-braces -std=c99 -g -O0 -I.
LDFLAGS = -Wall -Wextra -lm # -fsanitize=address

# Directories
LEXER_DIR = lexer
ERROR_DIR = error
AST_DIR = ast
PARSER_DIR = parser

# Source and object files for the lexer module
LEXER_SRCS = $(LEXER_DIR)/id_kw.c $(LEXER_DIR)/lexer.c $(LEXER_DIR)/string_lex.c
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
ERROR_SRCS = $(ERROR_DIR)/print_error.c
ERROR_OBJS = $(ERROR_SRCS:.c=.o)
AST_SRCS = $(AST_DIR)/ast.c
AST_OBJS = $(AST_SRCS:.c=.o)
PARSER_SRCS = $(PARSER_DIR)/parse.c $(PARSER_DIR)/backend_parser.c $(PARSER_DIR)/actions/actions.c
PARSER_OBJS = $(PARSER_SRCS:.c=.o)

TARGET = main

MAIN_OBJ = main.o

file ?= tests/success/op.java

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ) $(ERROR_OBJS) -o $(TARGET)

# Compile main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Generic rule for compiling lexer .c -> .o
$(LEXER_DIR)/%.o: $(LEXER_DIR)/%.c $(LEXER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for compiling errors .c -> .o
$(ERROR_DIR)/%.o: $(ERROR_DIR)/%.c $(ERROR_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for compiling AST .c -> .o
$(AST_DIR)/%.o: $(AST_DIR)/%.c $(AST_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for compiling parser .c -> .o
$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(PARSER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program with a test file
run: $(TARGET)
	./$(TARGET) $(file)

debugAll: all
	./$(TARGET) --debug-lexer --debug-parser $(file)

debugLexer: all
	./$(TARGET) --debug-lexer $(file)

debugParser: all
	./$(TARGET) --debug-parser $(file)

successTests: $(TARGET)
	@for file in tests/success/*; do \
		echo "Running $$file"; \
		if ! make run file=$$file; then \
			echo "Test $$file unexpectedly failed"; \
			exit 1; \
		fi; \
	done
	

failTests: $(TARGET)
	@for file in tests/fail/*; do \
		echo "Running $$file"; \
		if make run file=$$file; then \
			echo "Test $$file unexpectedly succeeded"; \
			exit 1; \
		fi; \
	done

# Clean build artifacts
clean:
	rm -f $(LEXER_DIR)/*.o main.o $(TARGET) $(ERROR_DIR)/*.o $(AST_DIR)/*.o $(PARSER_DIR)/*.o $(PARSER_DIR)/actions/*.o

# Rebuild everything
rebuild: clean all

.PHONY: all run clean rebuild