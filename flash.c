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

  struct Card deck[MAX_DECK_SIZE];
  struct FlashOpts opts = get_opts(argc, argv);

  size_t loaded = load_cards(opts.filepath, deck, MAX_DECK_SIZE);

  if (loaded == 0) {
    printf("flash: no cards found in deck\n");
    exit(0);
  }

  const char *filename = get_filename(opts.filepath);

  if (!opts.no_shuffle) {
    shuffle_cards(deck, loaded);
  }

  enter_alternate_buffer();

  int start_new = print_summary(filename);

  if (!start_new) {
    exit(0);
  }

  for (size_t i = 0; i < loaded; i++) {
    quiz_card(&deck[i], i + 1, loaded);
  }

  int score = get_score(deck, loaded);
  show_score(score, loaded);
  save_to_session(score, loaded, filename);
  return 0;
}
