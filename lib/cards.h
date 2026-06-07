#ifndef CARDS_H
#define CARDS_H

#include <stddef.h>
#include "term.h"

struct Card {
  char question[MAX_LINE_SIZE];
  char answer[MAX_LINE_SIZE];
  int correct;
};

enum CardState { SEPARATOR, QUESTION, ANSWER };


size_t load_cards(const char *filename, struct Card *out, size_t capacity);
void shuffle_cards(struct Card *deck, size_t size);
void quiz_card(struct Card *card, size_t index, size_t total);

#endif
