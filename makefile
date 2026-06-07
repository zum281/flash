.PHONY: clean

CC=clang
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c23

SRC=flash.c lib/*.c
OUT=flash

$(OUT):$(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
