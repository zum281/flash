#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FILE *fp;
  char s[100];
  fp = fopen("./cards.txt", "r");

  while (fgets(s, 100, fp) != NULL) {
    printf("%s\n", s);
  }

  fclose(fp);
  return 0;
}
