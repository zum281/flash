#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cards.h"

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

    // Skip new lines
    if (!strcmp(s, "\n")) {
      continue;
    }

    // skip empty lines
    if (strlen(s) == 0) {
      continue;
    }
    // strip away \n
    char *last_c = &s[strlen(s) - 1];
    if (*last_c == '\n') {

      *last_c = '\0';
    }

    // Skip separator
    if (!strcmp(s, separator)) {
      state = QUESTION;
      continue;
    }

    switch (state) {
    case SEPARATOR:
      if (strcmp(s, separator)) {
        fprintf(stderr, "flash: expected SEPARATOR, got %s. exiting\n", s);
        exit(1);
      }
      break;
    case QUESTION:
      strcpy(out[loaded].question, s);
      break;
    case ANSWER:
      strcpy(out[loaded].answer, s);
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
  char buf[10];
  char c = '\0';
  int correct;

  printf("Card %zu of %zu - press Enter to see the answer\n\n", index, total);
  printf("\t%s\n\n", card->question);
  printf("[Enter]");

  while (1) {
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      fprintf(stderr, "flash: no input, exiting\n");
      exit(1);
    }
    c = buf[0];
    if (c == '\n') {
      break;
    }
  }
  printf("\t%s\n\n", card->answer);

  printf("Got it? [y/n] ");
  while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      fprintf(stderr, "flash: no input, exiting\n");
      exit(1);
    }
    c = buf[0];
    switch (c) {
    case 'y':
    case 'Y':
      correct = 1;
      break;
    case 'n':
    case 'N':
      correct = 0;
      break;
    default:
      printf("Valid answers are y/n. Give your answer again [y/n] ");
    }
  };
  printf("\n\n");
  return correct;
}
