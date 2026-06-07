#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *log_filepath = "flash.log";

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

int validate_line(char *line) {

  if (line == NULL) {
    return 0;
  }

  if (strlen(line) == 0) {
    return 0;
  }
  if (!strcmp(line, " ") || !strcmp(line, "\n")) {
    return 0;
  }
  return 1;
}

char *sanitize_line(char *line) {
  // trim initial whitescpace
  while (*line == ' ')
    line++;

  // trim trailing whitespace
  char *last_c = &line[strlen(line) - 1];

  while (last_c >= line && (*last_c == ' ' || *last_c == '\n')) {
    *last_c = '\0';
    last_c--;
  }

  return line;
}
