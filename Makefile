build:
	gcc -Wall -fsanitize=address -fno-omit-frame-pointer -static-libasan -g -O0 lexer/lexer.c -o test

run:
	./test

clean:
	rm -rf test