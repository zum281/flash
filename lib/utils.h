#ifndef UTILS_H
#define UTILS_H

struct FlashOpts {
  char *filepath;
  int no_shuffle;
};

struct FlashOpts get_opts(int argc, char **argv);
const char *get_filename(const char *filepath);
void print_usage(int is_error);

#endif
