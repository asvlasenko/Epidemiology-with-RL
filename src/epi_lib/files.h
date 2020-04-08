// Common data file IO stuff
#ifndef __FILES_H__
#define __FILES_H__

#include "common.h"

#define MAX_LINE_SIZE 256

// Find a token in a data file.  The token name is a set of characters
// between '$' and end of line.  Value(s) follow on following lines.
// 0 indicates success.
EpiError find_token_in_file(FILE *fp, const char *token_name);

// Read natural number on line following a token name.
// 0 indicates success.
EpiError read_size_token(size_t *s, FILE *fp, const char *token_name);

// Read floating point number on line following a token name.
// 0 indicates success.
EpiError read_float_token(float *f, FILE *fp, const char *token_name);

// Read an array of floating point numbers on lines following a token name.
// 0 indicates success.
EpiError read_float_array(float *f, size_t size, FILE *fp, const char *token_name);

#endif
