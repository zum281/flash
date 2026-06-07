#ifndef SCORE_H
#define SCORE_H

#include <stddef.h>

int append_session(const char *log_filename, size_t total, size_t correct, const char *deck_name);
void show_score(int score, size_t total);
void save_to_session(int score, size_t total, const char *deck_name);

#endif
