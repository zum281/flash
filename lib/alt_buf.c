#include <stdio.h>
#include <stdlib.h>

#include "alt_buf.h"

int in_alternate = 0;

void exit_alternate_buffer(void) {
  if (in_alternate) {
    printf("\033[?25h");   // make cursor visible
    printf("\033[?1049l"); // exit alternate buffer
  }
}

void clear_screen(void) {
  printf("\033[2J"); // clear screen
  printf("\033[H");  // move cursor HOME (0,0)
}

void enter_alternate_buffer(void) {
  printf("\033[?25l");   // make cursor invisible
  printf("\033[?1049h"); // enter alternate buffer
  in_alternate = 1;
}
