#include "files.h"

epi_error_e read_size_token(size_t *s, FILE *fp, const char *token_name) {
  if (s == NULL || fp == NULL || token_name == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  PASS_ERROR(find_token_in_file(fp, token_name));

  char buf[MAX_LINE_SIZE];
  if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
    return EPI_ERROR_UNEXPECTED_EOF;
  }

  int result = atoi(buf);
  if (result <= 0) {
    return EPI_ERROR_INVALID_DATA;
  }

  *s = result;
  return EPI_ERROR_SUCCESS;
}

epi_error_e read_float_token(float *f, FILE *fp, const char *token_name) {
  if (f == NULL || fp == NULL || token_name == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  PASS_ERROR(find_token_in_file(fp, token_name));

  char buf[MAX_LINE_SIZE];
  if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
    return EPI_ERROR_UNEXPECTED_EOF;
  }

  double result = atof(buf);
  if ((result == 0.0 && buf[0] != '0' && buf[0] != '.') || result < 0.0) {
    return EPI_ERROR_INVALID_DATA;
  }

  *f = (float)result;
  return EPI_ERROR_SUCCESS;
}

epi_error_e read_float_array(float *f, size_t size, FILE *fp,
  const char *token_name) {
  if (f == NULL || size == 0 || fp == NULL || token_name == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  PASS_ERROR(find_token_in_file(fp, token_name));

  char buf[MAX_LINE_SIZE];
  for (size_t i = 0; i < size; i++) {
    if (fgets(buf, MAX_LINE_SIZE, fp) == NULL) {
      return EPI_ERROR_UNEXPECTED_EOF;
    }

    double result = atof(buf);
    if ((result == 0.0 && buf[0] != '0' && buf[0] != '.') || result < 0.0) {
      return EPI_ERROR_INVALID_DATA;
    }

    f[i] = (float)result;
  }

  return EPI_ERROR_SUCCESS;
}

epi_error_e find_token_in_file(FILE *fp, const char *token_name) {
  if (fp == NULL || token_name == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  char buf[MAX_LINE_SIZE];
  char tok[MAX_LINE_SIZE];
  sprintf(tok, "$%s\n", token_name);

  fseek(fp, 0, SEEK_SET);
  while (fgets(buf, MAX_LINE_SIZE, fp) != NULL) {
    if (!strcmp(buf, tok)) {
      return EPI_ERROR_SUCCESS;
    }
  }

  return EPI_ERROR_MISSING_DATA;
}
