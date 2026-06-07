#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

struct FlashOpts get_opts(int argc, char **argv) {
  struct FlashOpts opts;
  opts.filename = NULL;
  opts.no_shuffle = 0;

  for (int i = 1; i < argc; i++) {
    if (!strcmp("--no-shuffle", argv[i])) {
      opts.no_shuffle = 1;
    } else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
      print_usage(0);
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "flash: invalid argument '%s'\n", argv[i]);
      exit(1);
    } else if (opts.filename != NULL) {
      fprintf(stderr, "flash: too many arguments (unexpected '%s')\n", argv[i]);
      exit(1);
    } else {
      opts.filename = argv[i];
    }
  }

  if (opts.filename == NULL) {
    print_usage(1);
  }

  return opts;
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
