#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "score.h"
#include "term.h"
#include "utils.h"

#define SCORE_LINES 2
#define SUMMARY_LINES 2
#define MAX_LOG_SIZE 1000

int append_session(size_t total, size_t correct, const char *deck_name) {
  FILE *fp;
  fp = fopen(log_filepath, "a");

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

int get_score(struct Card *deck, size_t total) {
  int score = 0;

  for (size_t i = 0; i < total; i++) {
    score += deck[i].correct;
  }

  return score;
}

double get_score_perc(int score, size_t total) { return 100.0 * score / total; }

void show_score(int score, size_t total) {
  double score_perc = get_score_perc(score, total);

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

  if (!append_session(total, score, deck_name)) {
    print_line("Logged to flash.log");
  } else {
    perror("flash");
  }
}

int print_summary(const char *filename) {
  FILE *fp;
  char line[MAX_LINE_SIZE];

  fp = fopen(log_filepath, "r");

  if (fp == NULL) {
    perror("flash");
    exit(1);
  }

  int records_num = 0;
  double max_perc = 0.0;
  double avg_perc = 0.0;
  while (fgets(line, sizeof line, fp) != NULL) {
    if (records_num == MAX_LOG_SIZE) {
      printf(
          "Too many logs found: summary calculation done on first %d lines\n",
          MAX_LOG_SIZE);
      break;
    }

    strtok(line, "-");
    char *deck_tok = strtok(NULL, "-");

    char *score_str_tok = strtok(NULL, "-");

    int deck_valid = validate_line(deck_tok);
    int score_valid = validate_line(score_str_tok);

    if (!deck_valid || !score_valid) {
      continue;
    }

    deck_tok = sanitize_line(deck_tok);
    score_str_tok = sanitize_line(score_str_tok);

    // skip if deck is not the one provided
    if (strcmp(deck_tok, filename)) {
      continue;
    }

    // Get score and total
    char *score_tok = strtok(score_str_tok, "/");
    char *total_tok = strtok(NULL, "/");

    if (score_tok == NULL || total_tok == NULL) {
      continue;
    }

    int score = atoi(score_tok);
    int total = atoi(total_tok);
    double perc = get_score_perc(score, total);

    if (perc > max_perc) {
      max_perc = perc;
    }

    avg_perc += perc;

    records_num++;
  }

  fclose(fp);

  char summary_card[2][MAX_LINE_SIZE];
  if (records_num == 0) {

    snprintf(summary_card[0], sizeof(summary_card[0]),
             "No previous sessions for deck %s", filename);
  } else {
    avg_perc /= records_num;
    snprintf(summary_card[0], sizeof(summary_card[0]),
             "%s - %d sessions - avg %.1f%% - best %.2f%%", filename,
             records_num, avg_perc, max_perc);
  }

  snprintf(summary_card[1], sizeof(summary_card[1]),
           "Do you want to start a new game? [y/n]");

  print_card(summary_card, 2);

  return wait_for_yes_no();
}
