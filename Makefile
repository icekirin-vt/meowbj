CFLAGS= -Wall -Wextra -g  -std=c99

base:
	clang main.c $(CFLAGS) -o program
