CFLAGS= -Wall -Wextra -std=c99

basic: 
	clang $(CFLAGS) main.c -o program
