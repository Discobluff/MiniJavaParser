CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -static-libasan -std=c99 -g -O0
LDFLAGS = -Wall -Wextra -lm -fsanitize=address


run:
	./test


# Directories
LEXER_DIR = lexer

# Source and object files for the lexer module
LEXER_SRCS = $(LEXER_DIR)/id_kw.c $(LEXER_DIR)/lexer.c $(LEXER_DIR)/string_lex.c
LEXER_OBJS = $(LEXER_SRCS:.c=.o)

# Default target (build everything)
lexer: $(LEXER_OBJS)
	$(CC) $(LDFLAGS) $(LEXER_DIR)/*.o -o lexer_test

# Generic rule for compiling .c -> .o
$(LEXER_DIR)/%.o: $(LEXER_DIR)/%.c $(LEXER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(LEXER_DIR)/*.o

# Rebuild everything
rebuild: clean all
