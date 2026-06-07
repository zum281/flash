# flash

A terminal flashcard quiz program written in C.

This is a learning project, built while following
[Beej's Guide to C Programming](https://beej.us/guide/bgc/) (up to chapter 9).

Use at your own risk.

## What it does

- Loads a deck of flashcards from a plain text file
- Shuffles the deck (unless `--no-shuffle` is given)
- Quizzes you one card at a time: shows the question, waits for Enter, then
  reveals the answer and asks whether you got it right
- Repeats the cards you got wrong until you've cleared the deck
- Shows your score and (optionally) logs the session to `flash.log`
- On startup, summarises your past sessions for that deck (number of sessions,
  average and best score)

## Build

```sh
make
```

## Usage

```sh
flash [--no-shuffle] [-h|--help] <deck-file>
```

Examples:

```sh
flash decks/cards.txt
flash --no-shuffle ~/deck.txt
```

## Deck file format

Cards are plain text. A question line, an answer line, then a `---` separator:

```
What does sizeof return for an array parameter inside a function?
The size of the pointer it decayed to, not the original array size.
---
Why does fgetc return int rather than char?
So EOF (typically -1) is distinguishable from any valid character value.
---
```

Blank lines are ignored, and leading/trailing whitespace is trimmed. A deck can
hold up to 100 cards (`MAX_DECK_SIZE`).

## Project layout

```
flash.c         entry point
lib/cards.c     deck loading, shuffling, quizzing a single card
lib/score.c     scoring, session logging, startup summary
lib/term.c      drawing cards centered in the terminal, reading y/n & Enter
lib/alt_buf.c   alternate screen buffer handling
lib/utils.c     argument parsing, line validation and sanitization
flash.log       log of past sessions
```

## Notes

- The program switches to the terminal's _alternate screen buffer_ on start
  (the same trick `vim` and `less` use) and restores your screen on exit via an
  `atexit` handler, so your scrollback is left untouched.
- Cards are drawn centered using the terminal size from `ioctl(TIOCGWINSZ)`.
- Session history is parsed back out of `flash.log` with `strtok` to compute the
  startup summary.

## Known Issues

- If the program is killed abruptly (e.g. `Ctrl-C`), the cleanup that re-shows
  the cursor never runs, so your terminal cursor stays hidden. Restore it with:

  ```sh
  printf '\033[?25h'
  ```
