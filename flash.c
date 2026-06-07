#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "lib/alt_buf.h"
#include "lib/cards.h"
#include "lib/score.h"
#include "lib/term.h"
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

  int play = 1;
  int is_first_round = 1;
  int score = 0;

  while (play) {
    for (size_t i = 0; i < loaded; i++) {
      if (deck[i].correct == 1) {
        continue;
      }
      quiz_card(&deck[i], i + 1, loaded);
    }

    if (is_first_round) {
      score = get_score(deck, loaded);
      is_first_round = 0;
    }

    clear_screen();
    show_score(score, loaded);
    print_line("Play again the cards you got wrong? [y/n]");

    if (get_score(deck, loaded) == (int)loaded) {
      play = 0;
    } else {
      play = wait_for_yes_no();
    }
  }

  show_score(score, loaded);
  save_to_session(score, loaded, filename);
  return 0;
}
