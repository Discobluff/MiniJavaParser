CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0 -I.
LDFLAGS = -Wall -Wextra -lm # -fsanitize=address

# Directories
LEXER_DIR = lexer
ERROR_DIR = error
AST_DIR = ast
PARSER_DIR = demo_parser

# Source and object files for the lexer module
LEXER_SRCS = $(LEXER_DIR)/id_kw.c $(LEXER_DIR)/lexer.c $(LEXER_DIR)/string_lex.c
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
ERROR_SRCS = $(ERROR_DIR)/print_error.c
ERROR_OBJS = $(ERROR_SRCS:.c=.o)
AST_SRCS = $(AST_DIR)/ast.c
AST_OBJS = $(AST_SRCS:.c=.o)
PARSER_SRCS = $(PARSER_DIR)/parse.c
PARSER_OBJS = $(PARSER_SRCS:.c=.o)

MAIN_OBJ = main.o

# Default target
all: lexer_test

# Build the executable
lexer_test: $(ERROR_OBJS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(AST_OBJS) $(PARSER_OBJS) $(MAIN_OBJ) $(ERROR_OBJS) -o lexer_test

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
test: lexer_test
	./lexer_test test.java

# Clean build artifacts
clean:
	rm -f $(LEXER_DIR)/*.o main.o lexer_test $(ERROR_DIR)/*.o $(AST_DIR)/*.o $(PARSER_DIR)/*.o

# Rebuild everything
rebuild: clean all

.PHONY: all run clean rebuild