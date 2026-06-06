#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 128
#define MAX_DECK_SIZE 100

struct Card {
  char question[MAX_LINE_SIZE];
  char answer[MAX_LINE_SIZE];
  int correct;
};
enum CardState { SEPARATOR, QUESTION, ANSWER };

size_t load_cards(const char *filename, struct Card *out, size_t capacity);

int main(void) {
  size_t loaded;
  struct Card deck[MAX_DECK_SIZE];
  // TODO: get from args
  char *filename = "./cards.txt";

  loaded = load_cards(filename, deck, MAX_DECK_SIZE);

  printf("%zu\n", loaded);

  return 0;
}

size_t load_cards(const char *filename, struct Card *out, size_t capacity) {
  (void)out;
  const char *separator = "---";
  enum CardState state = QUESTION;

  FILE *fp;
  char s[MAX_LINE_SIZE];
  fp = fopen(filename, "r");

  while (fgets(s, MAX_LINE_SIZE, fp) != NULL) {
    // TODO: strip new lines
    if (!strcmp(s, separator)) {
      state = QUESTION;
      continue;
    }

    switch (state) {
    case SEPARATOR:
      break;
    case QUESTION:
      // TODO: parse question
      printf("Q: %s\n", s);
      break;
    case ANSWER:
      // TODO: parse answer
      printf("A: %s\n", s);
      break;
    default:
      printf("unrecognised state");
      break;
    }
    if (state == ANSWER)
      state = SEPARATOR;
    else
      state++;
  }

  fclose(fp);
  return capacity;
}
