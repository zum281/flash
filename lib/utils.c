#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

struct FlashOpts get_opts(int argc, char **argv) {
  struct FlashOpts opts;
  opts.filepath = NULL;
  opts.no_shuffle = 0;

  for (int i = 1; i < argc; i++) {
    if (!strcmp("--no-shuffle", argv[i])) {
      opts.no_shuffle = 1;
    } else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
      print_usage(0);
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "flash: invalid argument '%s'\n", argv[i]);
      exit(1);
    } else if (opts.filepath != NULL) {
      fprintf(stderr, "flash: too many arguments (unexpected '%s')\n", argv[i]);
      exit(1);
    } else {
      opts.filepath = argv[i];
    }
  }

  if (opts.filepath == NULL) {
    print_usage(1);
  }

  return opts;
}

const char *get_filename(const char *filepath) {
  const char *last_slash = strrchr(filepath, '/');

  if (last_slash) {
    return last_slash + 1;
  }
  return filepath;
}

void print_usage(int is_error) {
  char *usage = "usage: flash [--no-shuffle] [-h|--help] <cards>\n";
  if (is_error) {
    fprintf(stderr, "%s", usage);
    exit(1);
  }
  printf("%s", usage);
  exit(0);
}
