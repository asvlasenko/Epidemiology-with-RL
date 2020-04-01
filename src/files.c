#include "files.h"

int read_size_token(size_t *s, FILE *fp, const char *token_name) {
  if (find_token_in_file(fp, token_name)) {
    printf("Error: no %s token in file", token_name);
    return 1;
  }

  char buf[MAX_LINE_SIZE];
  if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
    printf("Error: unexpected EOF\n");
    return 1;
  }

  int result = atoi(buf);
  if (result <= 0) {
    printf("Error: invalid or missing size token value\n");
    return 1;
  }

  *s = result;
  return 0;
}

int read_float_token(float *f, FILE *fp, const char *token_name) {
  if (find_token_in_file(fp, token_name)) {
    printf("Error: no %s token in file", token_name);
    return 1;
  }

  char buf[MAX_LINE_SIZE];
  if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
    printf("Error: unexpected EOF\n");
    return 1;
  }

  double result = atof(buf);
  if ((result == 0.0 && buf[0] != '0' && buf[0] != '.') || result < 0.0) {
    printf("Error: invalid or missing value for %s\n", token_name);
    return 1;
  }

  *f = (float)result;
  return 0;
}

int read_float_array(float *f, size_t size, FILE *fp, const char *token_name) {
  if (find_token_in_file(fp, token_name)) {
    printf("Error: no %s token in file", token_name);
    return 1;
  }

  char buf[MAX_LINE_SIZE];
  for (size_t i = 0; i < size; i++) {
    if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
      printf("Error: unexpected EOF\n");
      return 1;
    }

    double result = atof(buf);
    if ((result == 0.0 && buf[0] != '0' && buf[0] != '.') || result < 0.0) {
      printf("Error: invalid or missing value for %s\n", token_name);
      return 1;
    }

    f[i] = (float)result;
  }

  return 0;
}

int find_token_in_file(FILE *fp, const char *token_name) {
  char buf[MAX_LINE_SIZE];
  char tok[MAX_LINE_SIZE];
  sprintf(tok, "$%s\n", token_name);

  fseek(fp, 0, SEEK_SET);
  while (fgets(buf, MAX_LINE_SIZE, fp) != NULL) {
    if (!strcmp(buf, tok)) {
      return 0;
    }
  }

  return 1;
}
