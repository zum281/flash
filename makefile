.PHONY: clean

CC=clang
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c23

SRC=flash.c
OUT = $(SRC:.c=)

$(OUT):$(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
