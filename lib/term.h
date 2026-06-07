#ifndef TERM_H
#define TERM_H

#define MAX_LINE_SIZE 256

#include <stddef.h>
#include <sys/ioctl.h>

int get_row_width(char *row, int padding);
int get_row_height(char *row, int padding, int new_lines, struct winsize ws);
int get_card_height(char rows[][MAX_LINE_SIZE], size_t size, int padding,
                    int new_lines, struct winsize ws);
void print_line(char *s);
void print_card(char rows[][MAX_LINE_SIZE], size_t size);

#endif
