CFLAGS= -Wall -Wextra -ggdb -fsanitize=address,undefined -fsanitize=address -std=c99

base:
	clang main.c $(CFLAGS) -o program
