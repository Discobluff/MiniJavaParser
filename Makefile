CC = gcc
CFLAGS = -Wall -fsanitize=address -fno-omit-frame-pointer -static-libasan -g -O0

build:
	$(CC) $(CFLAGS) lexer/lexer.c -o test

run:
	./test

clean:
	rm -rf test