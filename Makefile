CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0 -I.
LDFLAGS = -Wall -Wextra -lm -fsanitize=address

# Directories
LEXER_DIR = lexer

# Source and object files
LEXER_SRCS = $(LEXER_DIR)/id_kw.c $(LEXER_DIR)/lexer.c
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
MAIN_OBJ = main.o

# Default target
all: lexer_test

# Build the executable
lexer_test: $(LEXER_OBJS) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) $(LEXER_OBJS) $(MAIN_OBJ) -o lexer_test

# Compile main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Generic rule for compiling lexer .c -> .o
$(LEXER_DIR)/%.o: $(LEXER_DIR)/%.c $(LEXER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
test: lexer_test
	./lexer_test test.java

# Clean build artifacts
clean:
	rm -f $(LEXER_DIR)/*.o main.o lexer_test

# Rebuild everything
rebuild: clean all

.PHONY: all run clean rebuild