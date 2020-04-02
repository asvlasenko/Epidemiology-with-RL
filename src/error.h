#ifndef __ERROR_H__
#define __ERROR_H__

// Runtime error handling

typedef enum {
  ERROR_SUCCESS = 0,
  ERROR_INVALID_ARGS,
  ERROR_FILE_NOT_FOUND,
  ERROR_OUT_OF_MEMORY,
  ERROR_UNEXPECTED_STATE,
  ERROR_MISSING_DATA,
  ERROR_UNEXPECTED_EOF,
  ERROR_INVALID_DATA,
  N_ERROR
} error_e;

#define PASS_ERROR(expr) \
  {error_e err = expr; if(err != ERROR_SUCCESS) return err;}

#endif
