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
  int state = 2; // assume start with question; start with sep is handled

  FILE *fp;
  char s[MAX_LINE_SIZE];
  fp = fopen(filename, "r");

  while (fgets(s, MAX_LINE_SIZE, fp) != NULL) {
    // TODO: strip new lines
    if (!strcmp(s, separator)) {
      state = 1; // EXPECT_QUESTION
      continue;
    }

    switch (state) {
    case 1:
      break;
    case 2: // parse question
      printf("Q: %s\n", s);
      break;
    case 3:
      // parse answer
      printf("A: %s\n", s);
      break;
    default:
      printf("unrecognised state");
      break;
    }
    if (state == 3)
      state = 1;
    else
      state++;
  }

  fclose(fp);
  return capacity;
}
