#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "lib/alt_buf.h"
#include "lib/cards.h"
#include "lib/score.h"
#include "lib/utils.h"

#define MAX_DECK_SIZE 100

int main(int argc, char **argv) {
  atexit(exit_alternate_buffer);

  struct FlashOpts opts = get_opts(argc, argv);

  size_t loaded;
  int score = 0;
  struct Card deck[MAX_DECK_SIZE];

  loaded = load_cards(opts.filename, deck, MAX_DECK_SIZE);

  if (loaded == 0) {
    printf("No cards loaded, exiting...\n");
    exit(0);
  }

  if (!opts.no_shuffle) {
    shuffle_cards(deck, loaded);
  }

  enter_alternate_buffer();

  for (size_t i = 0; i < loaded; i++) {
    score += quiz_card(&deck[i], i + 1, loaded);
  }

  show_score(score, loaded);
  save_to_session(score, loaded);
  return 0;
}
