# Compiler and flags
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
TYPE_CHECKER_DIR = typeChecker

# Source and object files
LEXER_SRCS = $(LEXER_DIR)/id_kw.c $(LEXER_DIR)/lexer.c $(LEXER_DIR)/string_lex.c
LEXER_OBJS = $(LEXER_SRCS:.c=.o)

ERROR_SRCS = $(ERROR_DIR)/print_error.c
ERROR_OBJS = $(ERROR_SRCS:.c=.o)
AST_SRCS = $(AST_DIR)/ast.c $(AST_DIR)/displayAst.c
AST_OBJS = $(AST_SRCS:.c=.o)
PARSER_SRCS = $(PARSER_DIR)/parse.c 
PARSER_OBJS = $(PARSER_SRCS:.c=.o)

# Sources et objects files for the type_checker
TYPE_CHECKER_SCRS = typeChecker/type_checker.c
TYPE_CHECKER_OBJS = $(TYPE_CHECKER_SCRS:.c=.o)

# Valgrind memckeck flags
MEMCHECK_FLAGS = --leak-check=full --errors-for-leak-kinds=definite --error-exitcode=1

TARGET = main
MAIN_OBJ = main.o

# Test binaries
TESTPARSE_SRC = parser/testparse.c
TESTPARSE_OBJ = parser/testparse.o
TESTPARSE_BIN = testparse

PRINTAST_SRC = ast/print_ast_main.c
PRINTAST_OBJ = ast/print_ast_main.o
PRINTAST_BIN = printast

# Default test file
file ?= tests/implemented/syntax/success/call1.java

# Valgrind flags
MEMCHECK_FLAGS = --leak-check=full --errors-for-leak-kinds=definite --error-exitcode=1

# ============================================================================
# Build Targets
# ============================================================================

.PHONY: all clean rebuild

all: $(TARGET)

# Build the executable
$(TARGET): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(TYPE_CHECKER_OBJS) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(TYPE_CHECKER_OBJS) $(MAIN_OBJ) $(ERROR_OBJS) -o $(TARGET) $(LDFLAGS_SDL) 

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTPARSE_BIN): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(TESTPARSE_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(TESTPARSE_OBJ): $(TESTPARSE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(PRINTAST_BIN): $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) ast/print_ast.o $(PARSER_OBJS) $(PRINTAST_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

displayAst: ast/ast.o ast/displayAst.o ast/astExample.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDFLAGS_SDL)

# ============================================================================
# Pattern Rules
# ============================================================================

$(LEXER_DIR)/%.o: $(LEXER_DIR)/%.c $(LEXER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ERROR_DIR)/%.o: $(ERROR_DIR)/%.c $(ERROR_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(AST_DIR)/%.o: $(AST_DIR)/%.c $(AST_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(PARSER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

#Generic rule for compiling type_checker.c -> .o
$(TYPE_CHECKER_DIR)/%.o: $(TYPE_CHECKER_DIR)/%.c $(TYPE_CHECKER_DIR)/%.h 
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program with a test file
# ============================================================================
# Run and Debug Targets
# ============================================================================

.PHONY: run runDisplayAst debugAll debugLexer debugParser showAst

run: $(TARGET)
	./$(TARGET) $(file)

runDisplayAst: displayAst
	./displayAst

debugAll: all
	./$(TARGET) --debug-lexer --debug-parser $(file)

debugLexer: all
	./$(TARGET) --debug-lexer $(file)

debugParser: all
	./$(TARGET) --debug-parser $(file)

showAst: all
	./$(TARGET) --show-ast $(file)

typeChecker: all
	./$(TARGET) --typechecker $(file)

# ============================================================================
# Test Targets
# ============================================================================

.PHONY: successTests successMemory failTests failTodo successTodo

successTests: $(TARGET)
	@status=0; \
	for file in tests/implemented/*/success/*; do \
		printf "%-56s " "$$file"; \
		if ./main $$file > /dev/null 2>&1; then \
			echo "\033[38;5;77mOK\033[0m"; \
		else \
			echo "\033[38;5;196mFAIL\033[0m"; \
			status=1; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Successes has failed (some files failed)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mAll tests passed\033[0m\n"; \
	fi

successMemory: $(TARGET)
	@status=0; \
	for file in tests/implemented/*/success/*; do \
		printf "%-56s " "$$file"; \
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
		printf "%-69s " "$$file"; \
		if ./main $$file > /dev/null 2>&1; then \
			echo "\033[38;5;196mFAIL\033[0m"; \
			status=1; \
		else \
			echo "\033[38;5;77mOK\033[0m"; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Failures has failed (some file(s) succeeded)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mAll tests passed\033[0m\n"; \
	fi

failTypeTodo: $(TARGET)
	@status=0; \
	for file in tests/implemented/type/fail/*; do \
		printf "%-69s " "$$file"; \
		if ./main --typechecker $$file > /dev/null 2>&1; then \
			echo "\033[38;5;196mFAIL\033[0m"; \
			status=1; \
		else \
			echo "\033[38;5;77mOK\033[0m"; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Failures has failed (some file(s) succeeded)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mAll tests passed\033[0m\n"; \
	fi

successTypeTodo: $(TARGET)
	@status=0; \
	for file in tests/implemented/type/fail/*; do \
		printf "%-69s " "$$file"; \
		if ./main --typechecker $$file > /dev/null 2>&1; then \
			echo "\033[38;5;196mFAIL\033[0m"; \
			status=1; \
		else \
			echo "\033[38;5;77mOK\033[0m"; \
		fi; \
	done; \
	if [ $$status -ne 0 ]; then \
		echo "\n\t\t\033[38;5;196mTest Failures has failed (some file(s) succeeded)\033[0m\n"; \
		exit 1; \
	else \
		echo "\t\t\033[38;5;77mAll tests passed\033[0m\n"; \
	fi

# ============================================================================
# Cleanup
# ============================================================================

clean:
	rm -f $(LEXER_DIR)/*.o $(ERROR_DIR)/*.o $(AST_DIR)/*.o $(PARSER_DIR)/*.o
	rm -f $(MAIN_OBJ) $(TARGET)
	rm -f $(TESTPARSE_OBJ) $(TESTPARSE_BIN)
	rm -f $(PRINTAST_OBJ) $(PRINTAST_BIN)
	rm -f typeChecker/*.o

rebuild: clean all