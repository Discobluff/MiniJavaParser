build:
	gcc -Wall -fsanitize=address -static-libasan -g lexer/lexer.c -o test

run: build
	./test 1+1

clean:
	rm -rf test