CFLAGS= -Wextra -std=c99 -g

basic: 
	clang $(CFLAGS) main.c -o program
