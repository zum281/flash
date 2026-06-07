#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "score.h"
#include "term.h"

#define SCORE_LINES 2

int append_session(const char *filename, size_t total, size_t correct,
                   char *deck_name) {
  FILE *fp;
  fp = fopen(filename, "a");

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

void save_to_session(int score, size_t total, char *deck_name) {
  char buf[10];
  char save = '\0';

  print_line("Save to session? [y/n] ");

  while (save != 'y' && save != 'Y' && save != 'n' && save != 'N') {
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      fprintf(stderr, "flash: no input, exiting\n");
      exit(1);
    }

    save = buf[0];
    switch (save) {
    case 'y':
    case 'Y':
      if (!append_session("flash.log", total, score, deck_name)) {
        print_line("Logged to flash.log\n");
      } else {
        perror("flash");
      }
      break;
    case 'n':
    case 'N':
      break;
    default:
      print_line("Valid answers are y/n. Give your answer again [y/n] ");
    }
  };
}
