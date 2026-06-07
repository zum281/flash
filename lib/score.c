#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "score.h"
#include "term.h"

#define SCORE_LINES 2

int append_session(const char *log_filename, size_t total, size_t correct,
                   const char *deck_name) {
  FILE *fp;
  fp = fopen(log_filename, "a");

  if (fp == NULL) {
    return 1;
  }

  time_t ts = time(NULL);
  char *now = ctime(&ts);
  now[strlen(now) - 1] = '\0';

  fprintf(fp, "%s - %s - %zu/%zu\n", now, deck_name, correct, total);
  fclose(fp);
  return 0;
}

void show_score(int score, size_t total) {

  double score_perc = 100.0 * score / total;

  char rows[SCORE_LINES][MAX_LINE_SIZE];

  snprintf(rows[0], sizeof(rows[0]), "Session complete");
  snprintf(rows[1], sizeof(rows[1]), "Score: %d/%zu (%.1f%%)", score, total,
           score_perc);

  print_card(rows, SCORE_LINES);
}

void save_to_session(int score, size_t total, const char *deck_name) {
  print_line("Save to session? [y/n] ");

  int save = wait_for_yes_no();

  if (!save) {
    exit(0);
  }

  if (!append_session("flash.log", total, score, deck_name)) {
    print_line("Logged to flash.log\n");
  } else {
    perror("flash");
  }
}
