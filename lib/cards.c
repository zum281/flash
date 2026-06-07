#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "cards.h"
#include "term.h"
#include "utils.h"

#define QUESTION_LINES 3
#define ANSWER_LINES 2

size_t load_cards(const char *filename, struct Card *out, size_t capacity) {
  const char *separator = "---";
  enum CardState state = QUESTION;

  FILE *fp;
  char s[MAX_LINE_SIZE];
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("flash");
    exit(1);
  }

  size_t loaded = 0;

  while (fgets(s, MAX_LINE_SIZE, fp) != NULL) {
    if (loaded == capacity) {
      printf("capacity reached\n");
      break;
    }

    int line_valid = validate_line(s);
    if (!line_valid) {
      continue;
    }

    char *line = sanitize_line(s);

    // Skip separator
    if (!strcmp(line, separator)) {
      state = QUESTION;
      continue;
    }

    switch (state) {
    case SEPARATOR:
      if (strcmp(line, separator)) {
        fprintf(stderr, "flash: expected SEPARATOR, got '%s'\n", line);
        exit(1);
      }
      break;
    case QUESTION:
      strcpy(out[loaded].question, line);
      break;
    case ANSWER:
      strcpy(out[loaded].answer, line);
      loaded++;
      break;
    default:
      printf("unrecognised state\n");
      break;
    }
    if (state == ANSWER)
      state = SEPARATOR;
    else
      state++;
  }

  fclose(fp);
  return loaded;
}

void shuffle_cards(struct Card *deck, size_t size) {
  time_t now = time(NULL);
  srand((unsigned)now);

  for (size_t i = size - 1; i > 0; i--) {
    size_t j = rand() % size;
    struct Card tmp = deck[j];
    deck[j] = deck[i];
    deck[i] = tmp;
  }
}

int quiz_card(struct Card *card, size_t index, size_t total) {
  char question_rows[QUESTION_LINES][MAX_LINE_SIZE];
  char answer_rows[ANSWER_LINES][MAX_LINE_SIZE];

  snprintf(question_rows[0], sizeof(question_rows[0]),
           "Card %zu of %zu - press Enter to see the answer", index, total);
  snprintf(question_rows[1], sizeof(question_rows[1]), "%s", card->question);
  snprintf(question_rows[2], sizeof(question_rows[2]), "[Enter]");

  snprintf(answer_rows[0], sizeof(answer_rows[0]), "%s", card->answer);
  snprintf(answer_rows[1], sizeof(answer_rows[1]), "Got it? [y/n] ");

  print_card(question_rows, QUESTION_LINES);

  wait_for_enter();

  print_card(answer_rows, ANSWER_LINES);

  return wait_for_yes_no();
}
