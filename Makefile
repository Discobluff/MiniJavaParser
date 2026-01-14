CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0 -I.
LDFLAGS = -Wall -Wextra -lm
LDFLAGS_SDL = $(shell pkg-config --libs sdl2 SDL2_ttf)

# Suppress make's directory change messages
MAKEFLAGS += --no-print-directory

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
PARSER_SRCS = $(PARSER_DIR)/parse.c $(AST_DIR)/displayAst.c
PARSER_OBJS = $(PARSER_SRCS:.c=.o)

# Valgrind memckeck flags
MEMCHECK_FLAGS = --leak-check=full --errors-for-leak-kinds=definite --error-exitcode=1

TARGET = main

MAIN_OBJ = main.o

# New testparse target variables
TESTPARSE_SRC = parser/testparse.c
TESTPARSE_OBJ = parser/testparse.o
TESTPARSE_BIN = testparse

# Print AST test
PRINTAST_SRC = ast/print_ast_main.c
PRINTAST_OBJ = ast/print_ast_main.o
PRINTAST_BIN = printast

file ?= tests/implemented/syntax/success/call1.java

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ) $(ERROR_OBJS) -o $(TARGET) $(LDFLAGS_SDL) 

# Compile main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

runDisplayAst: displayAst
	./displayAst

displayAst: ast/ast.o ast/displayAst.o ast/astExample.o
	$(CC) $(LDFLAGS) ast/ast.o ast/displayAst.o ast/astExample.o -o $@ $(LDFLAGS_SDL)

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

# Compile and link testparse (uses existing objects)
$(TESTPARSE_BIN): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(TESTPARSE_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(TESTPARSE_OBJ) $(ERROR_OBJS) -o $(TESTPARSE_BIN)

# Compile and link printast: parses a file and prints AST to stdout
$(PRINTAST_BIN): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(PRINTAST_OBJ) ast/print_ast.o
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) ast/print_ast.o $(PARSER_OBJS) $(PRINTAST_OBJ) $(ERROR_OBJS) -o $(PRINTAST_BIN)

# Compile testparse.c -> testparse.o
$(TESTPARSE_OBJ): $(TESTPARSE_SRC)
	$(CC) $(CFLAGS) -c $(TESTPARSE_SRC) -o $(TESTPARSE_OBJ)

debugAll: all
	./$(TARGET) --debug-lexer --debug-parser $(file)

debugLexer: all
	./$(TARGET) --debug-lexer $(file)

debugParser: all
	./$(TARGET) --debug-parser $(file)

showAst: all
	./$(TARGET) --show-ast $(file)

successTests: $(TARGET)
	@status=0; \
	for file in tests/implemented/*/success/*; do \
		echo "Running $$file"; \
		if ! make run file=$$file; then \
			echo "Test $$file unexpectedly failed"; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Successes has failed (some files failed)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mTest Success succeeded\033[0m\n"; \
	fi

successMemory: $(TARGET)
	@status=0; \
	for file in tests/implemented/*/success/*; do \
		printf "%-55s " "$$file"; \
		if valgrind $(MEMCHECK_FLAGS) ./main $$file > /dev/null 2>&1; then \
			echo "\033[38;5;77mOK\033[0m"; \
		else \
			echo "\033[38;5;196mLEAK\033[0m"; \
			status=1; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mMemory check failed\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mAll memory checks passed\033[0m\n"; \
	fi

failTests: $(TARGET)
	@status=0; \
	for file in tests/implemented/*/fail/*; do \
		echo "Running $$file"; \
		if make run file=$$file; then \
			echo "Test $$file unexpectedly succeeded"; \
			status=1; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Failues has failed (some file(s) succeeded)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mTest Failures succeeded\033[0m\n"; \
	fi


failTodo: $(TARGET)
	@for file in tests/todo/syntax/fail/*; do \
		echo "Running $$file"; \
		if ! make run file=$$file; then \
			echo "Test $$file failed good"; \
			mv $$file tests/implemented/syntax/fail/ ; \
		fi; \
	done

successTodo: $(TARGET)
	@for file in tests/todo/*/success/*; do \
		echo "Running $$file"; \
		if ./main $$file; then \
			echo "Test $$file succeeded cool"; \
		fi; \
	done

# Clean build artifacts
clean:
	rm -f $(LEXER_DIR)/*.o main.o $(TARGET) $(ERROR_DIR)/*.o $(AST_DIR)/*.o $(PARSER_DIR)/*.o
	rm -f $(TESTPARSE_OBJ) $(TESTPARSE_BIN)
	rm -f $(PRINTAST_OBJ) $(PRINTAST_BIN)

# Rebuild everything
rebuild: clean all

.PHONY: all run clean rebuild