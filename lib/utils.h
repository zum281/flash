#ifndef UTILS_H
#define UTILS_H

extern char *log_filepath;

struct FlashOpts {
  char *filepath;
  int no_shuffle;
};

struct FlashOpts get_opts(int argc, char **argv);
const char *get_filename(const char *filepath);
void print_usage(int is_error);

int validate_line(char *line);
char *sanitize_line(char *line);

#endif
