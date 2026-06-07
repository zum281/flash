#define _DEFAULT_SOURCE

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_SIZE 128
#define MAX_DECK_SIZE 100

struct Card {
  char question[MAX_LINE_SIZE];
  char answer[MAX_LINE_SIZE];
};

enum CardState { SEPARATOR, QUESTION, ANSWER };
int in_alternate = 0;

size_t load_cards(const char *filename, struct Card *out, size_t capacity);
void shuffle_cards(struct Card *deck, size_t size);
int quiz_card(struct Card *card, size_t index, size_t total);
int append_session(const char *log_filename, size_t total, size_t correct);
void exit_alternate_buffer(void) {
  if (in_alternate) {
    printf("\033[?1049l");
  }
}

int main(int argc, char **argv) {

  atexit(exit_alternate_buffer);

  if (argc < 2) {
    printf("usage: flash <cards>\n");
    return 1;
  }
  char *filename = argv[1];
  size_t loaded;
  int score = 0;
  struct Card deck[MAX_DECK_SIZE];

  loaded = load_cards(filename, deck, MAX_DECK_SIZE);

  if (loaded == 0) {
    printf("No cards loaded, exiting...\n");
    exit(0);
  }

  shuffle_cards(deck, loaded);

  printf("loaded %zu cards\n", loaded);

  // Enter alternate buffer
  printf("\033[?1049h");
  in_alternate = 1;

  for (size_t i = 0; i < loaded; i++) {
    score += quiz_card(&deck[i], i + 1, loaded);
  }

  double score_perc = 100.0 * score / loaded;

  printf("──────────────────────────────────────\n");
  printf("Session complete.\n");
  printf("Score: %d/%zu (%.1f%%)\n", score, loaded, score_perc);

  printf("\n");
  printf("Save to session? [y/n] ");

  char buf[10];
  char save = '\0';
  while (save != 'y' && save != 'Y' && save != 'n' && save != 'N') {
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
      fprintf(stderr, "flash: no input, exiting\n");
      exit(1);
    }
    save = buf[0];
    switch (save) {
    case 'y':
    case 'Y':
      if (!append_session("flash.log", loaded, score)) {
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
  return 0;
}

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

    // strip away \n
    s[strlen(s) - 1] = '\0';

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
