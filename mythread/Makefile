CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -fsanitize=leak -fPIC -Iinclude
LDFLAGS = -shared -fsanitize=address -fsanitize=leak
LIB = libmythread.so
TEST = test

SRC = src/mythread.c
OBJ = src/mythread.o

all: $(LIB) $(TEST)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(TEST): test.c $(LIB)
	$(CC) $(CFLAGS) -o $@ test.c -L. -lmythread -Wl,-rpath,.

clean:
	rm -f $(OBJ) $(LIB) $(TEST)

.PHONY: all clean
