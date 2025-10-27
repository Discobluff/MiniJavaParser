# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I. -I..
LDFLAGS = 

# Directories
PARSER_DIR = .
AST_DIR = ../ast
ERROR_DIR = ../error
LEXER_DIR = ../lexer
TEST_DIR = test_parser

# Source files
PARSER_SRCS = $(PARSER_DIR)/backend_parser.c \
              $(PARSER_DIR)/parse.c \

AST_SRCS = $(AST_DIR)/ast.c

ERROR_SRCS = $(ERROR_DIR)/print_error.c

LEXER_SRCS = $(LEXER_DIR)/lexer.c \
             $(LEXER_DIR)/id_kw.c \
             $(LEXER_DIR)/string_lex.c

# All object files
PARSER_OBJS = $(PARSER_SRCS:.c=.o)
AST_OBJS = $(AST_SRCS:.c=.o)
ERROR_OBJS = $(ERROR_SRCS:.c=.o)
LEXER_OBJS = $(LEXER_SRCS:.c=.o)

ALL_OBJS = $(PARSER_OBJS) $(AST_OBJS) $(ERROR_OBJS) $(LEXER_OBJS)

# Test files
TEST_BACKEND_SRC = $(TEST_DIR)/test_backend_parser.c
TEST_SRC = $(TEST_DIR)/test_parse.c

TEST_BACKEND_OBJ = $(TEST_BACKEND_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

TEST_BACKEND_EXEC = $(TEST_DIR)/test_backend_parser
TEST_EXEC = $(TEST_DIR)/test_parse

# Default target
all: $(TEST_BACKEND_EXEC) $(TEST_EXEC)

# Build backend test executable
$(TEST_BACKEND_EXEC): $(ALL_OBJS) $(TEST_BACKEND_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build refonte test executable
$(TEST_EXEC): $(ALL_OBJS) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run all tests
test: $(TEST_BACKEND_EXEC) $(TEST_EXEC)
	@echo "\n=========================================="
	@echo "Running Backend Parser Tests"
	@echo "==========================================\n"
	./$(TEST_BACKEND_EXEC)
	@echo "\n=========================================="
	@echo "Running Parse Refonte Tests"
	@echo "==========================================\n"
	./$(TEST_EXEC)

# Run individual tests (compile if needed)
test-backend: $(TEST_BACKEND_EXEC)
	@echo "\n=========================================="
	@echo "Running Backend Parser Tests"
	@echo "==========================================\n"
	./$(TEST_BACKEND_EXEC)

test-refonte: $(TEST_EXEC)
	@echo "\n=========================================="
	@echo "Running Parse Refonte Tests"
	@echo "==========================================\n"
	./$(TEST_EXEC)

# Run tests with valgrind for memory leak detection
memcheck: $(TEST_BACKEND_EXEC) $(TEST_EXEC)
	@echo "\n=========================================="
	@echo "Memory Check: Backend Parser Tests"
	@echo "==========================================\n"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BACKEND_EXEC)
	@echo "\n=========================================="
	@echo "Memory Check: Parse Refonte Tests"
	@echo "==========================================\n"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_EXEC)

# Individual memory checks
memcheck-backend: $(TEST_BACKEND_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BACKEND_EXEC)

memcheck-refonte: $(TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_EXEC)

# Clean build artifacts
clean:
	rm -f $(ALL_OBJS) $(TEST_BACKEND_OBJ) $(TEST_OBJ)
	rm -f $(TEST_BACKEND_EXEC) $(TEST_EXEC)
	rm -f $(PARSER_DIR)/*.o
	rm -f $(AST_DIR)/*.o
	rm -f $(ERROR_DIR)/*.o
	rm -f $(LEXER_DIR)/*.o
	rm -f $(TEST_DIR)/*.o

# Clean and rebuild
rebuild: clean all

# Show variables (for debugging the Makefile)
show:
	@echo "PARSER_SRCS: $(PARSER_SRCS)"
	@echo "AST_SRCS: $(AST_SRCS)"
	@echo "ERROR_SRCS: $(ERROR_SRCS)"
	@echo "LEXER_SRCS: $(LEXER_SRCS)"
	@echo "ALL_OBJS: $(ALL_OBJS)"
	@echo "TEST_BACKEND_EXEC: $(TEST_BACKEND_EXEC)"
	@echo "TEST_EXEC: $(TEST_EXEC)"

# Phony targets
.PHONY: all test test-backend test-refonte memcheck memcheck-backend memcheck-refonte clean rebuild show