#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "score.h"

int append_session(const char *filename, size_t total, size_t correct) {
  FILE *fp;
  fp = fopen(filename, "a");

  if (fp == NULL) {
    return 1;
  }

  time_t ts = time(NULL);
  char *now = ctime(&ts);
  now[strlen(now) - 1] = '\0';

  fprintf(fp, "%s - %zu/%zu\n", now, correct, total);
  fclose(fp);
  return 0;
}

void show_score(int score, size_t total) {

  double score_perc = 100.0 * score / total;

  printf("Session complete.\n");
  printf("Score: %d/%zu (%.1f%%)\n", score, total, score_perc);

  printf("\n");
}

void save_to_session(int score, size_t total) {
  char buf[10];
  char save = '\0';
  printf("Save to session? [y/n] ");

  while (save != 'y' && save != 'Y' && save != 'n' && save != 'N') {
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      fprintf(stderr, "flash: no input, exiting\n");
      exit(1);
    }
    save = buf[0];
    switch (save) {
    case 'y':
    case 'Y':
      if (!append_session("flash.log", total, score)) {
        printf("Logged to flash.log\n");
      } else {
        perror("flash");
      }
      break;
    case 'n':
    case 'N':
      break;
    default:
      printf("Valid answers are y/n. Give your answer again [y/n] ");
    }
  };
}
