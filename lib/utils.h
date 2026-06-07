#ifndef UTILS_H
#define UTILS_H

struct FlashOpts {
  char *filename;
  int no_shuffle;
};

struct FlashOpts get_opts(int argc, char **argv);
void print_usage(int is_error);

#endif
