#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alt_buf.h"
#include "term.h"

int get_row_width(char *row, int padding) { return strlen(row) + padding; }
int get_row_height(char *row, int padding, int new_lines, struct winsize ws) {
  return (get_row_width(row, padding) + ws.ws_col - 1) / ws.ws_col + new_lines;
}

int get_card_height(char rows[][MAX_LINE_SIZE], size_t size, int padding,
                    int new_lines, struct winsize ws) {
  int sum = 0;
  for (size_t i = 0; i < size; i++) {
    int new_lines_for_row = i == size - 1 ? 0 : new_lines;
    int row_height = get_row_height(rows[i], padding, new_lines_for_row, ws);
    sum += row_height;
  }
  return sum + padding;
}

void print_line(char *s) {
  int padding = 2;
  char line[MAX_LINE_SIZE];

  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    perror("flash");
    exit(1);
  }

  snprintf(line, sizeof(line), "%s", s);

  int start_col = (ws.ws_col - get_row_width(line, padding)) / 2;

  printf("\033[%dG", start_col);
  printf("%s", line);
}

void print_card(char rows[][MAX_LINE_SIZE], size_t size) {
  int padding = 2;
  int new_lines = 2;
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    perror("flash");
    exit(1);
  }

  clear_screen();
  int start_print_a_row =
      ((ws.ws_row - get_card_height(rows, size, padding, new_lines, ws)) / 2);

  for (size_t i = 0; i < size; i++) {
    int start_print_a_col = (ws.ws_col - get_row_width(rows[i], padding)) / 2;
    if (i == 0) {
      printf("\033[%d;%dH", start_print_a_row, start_print_a_col);
    } else {

      printf("\033[%dG", start_print_a_col);
    }
    printf("%s", rows[i]);
    if (i < size - 1) {
      printf("\n\n");
    }
  }
}
