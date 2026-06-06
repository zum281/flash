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
  int correct;
};

enum CardState { SEPARATOR, QUESTION, ANSWER };

size_t load_cards(const char *filename, struct Card *out, size_t capacity);
void shuffle_cards(struct Card *deck, int size);
int quiz_card(struct Card card, size_t index, size_t total);

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("usage: flash <cards>\n");
    return 1;
  }
  char *filename = argv[1];
  size_t loaded;
  int score = 0;
  struct Card deck[MAX_DECK_SIZE];

  loaded = load_cards(filename, deck, MAX_DECK_SIZE);

  shuffle_cards(deck, (int)loaded);

  printf("loaded %zu cards\n", loaded);

  for (size_t i = 0; i < loaded; i++) {
    score += quiz_card(deck[i], i + 1, loaded);
  }

  double scorePerc = 100.0 * score / loaded;

  printf("──────────────────────────────────────\n");
  printf("Session complete.\n");
  printf("Score: %d/%zu (%.1f%%)\n", score, loaded, scorePerc);
  // TODO: log to file
  // Logged to flash.log
  return 0;
}

size_t load_cards(const char *filename, struct Card *out, size_t capacity) {
  const char *separator = "---";
  enum CardState state = QUESTION;

  FILE *fp;
  char s[MAX_LINE_SIZE];
  fp = fopen(filename, "r");

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

    // Skip separator
    if (!strcmp(s, separator)) {
      state = QUESTION;
      continue;
    }

    s[strlen(s) - 1] = '\0';
    switch (state) {
    case SEPARATOR:
      break;
    case QUESTION:
      strcpy(out[loaded].question, s);
      break;
    case ANSWER:
      strcpy(out[loaded].answer, s);
      out[loaded].correct = 0;
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

void shuffle_cards(struct Card *deck, int size) {
  time_t now = time(NULL);
  srand((unsigned)now);

  for (int i = size - 1; i > 0; i--) {
    int j = rand() % size;
    struct Card tmp = deck[j];
    deck[j] = deck[i];
    deck[i] = tmp;
  }
}

int quiz_card(struct Card card, size_t index, size_t total) {
  char buf[10];
  char c = '\0';
  int correct;

  printf("Card %zu of %zu - press Enter to see the answer\n\n", index, total);
  printf("\t%s\n\n", card.question);
  printf("[Enter]");

  while (1) {
    fgets(buf, sizeof(buf), stdin);
    c = buf[0];

    if ((int)c == 10) {
      break;
    }
  }
  printf("\t%s\n\n", card.answer);

  printf("Got it? [y/n] ");
  while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
    fgets(buf, sizeof(buf), stdin);
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
