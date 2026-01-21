CFLAGS= -Wextra -Werror -std=c99

basic: 
	clang $(CFLAGS) main.c -o program
