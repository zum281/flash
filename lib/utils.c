#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

char *get_filename(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: flash <cards>\n");
    exit(1);
  }
  return argv[1];
}
