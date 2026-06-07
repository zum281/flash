#ifndef CARDS_H
#define CARDS_H

#include <stddef.h>

#define MAX_LINE_SIZE 256
#define MAX_DECK_SIZE 100

struct Card {
  char question[MAX_LINE_SIZE];
  char answer[MAX_LINE_SIZE];
};

enum CardState { SEPARATOR, QUESTION, ANSWER };

size_t load_cards(const char *filename, struct Card *out, size_t capacity);
void shuffle_cards(struct Card *deck, size_t size);
int quiz_card(struct Card *card, size_t index, size_t total);

#endif
