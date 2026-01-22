CFLAGS= -Wextra -std=c99 -O2

basic: 
	clang $(CFLAGS) main.c -o program
