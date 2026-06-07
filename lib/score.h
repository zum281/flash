#ifndef SCORE_H
#define SCORE_H

#include <stddef.h>
#include "cards.h"

int append_session(size_t total, size_t correct, const char *deck_name);
int get_score(struct Card *deck, size_t total);
void show_score(int score, size_t total);
void save_to_session(int score, size_t total, const char *deck_name);
int print_summary(const char *filename);

#endif
