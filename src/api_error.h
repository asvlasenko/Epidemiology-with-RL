#ifndef __ERROR_H__
#define __ERROR_H__

// Runtime error handling

typedef enum {
  ERROR_SUCCESS = 0,
  ERROR_INVALID_ARGS,
  ERROR_FILE_NOT_FOUND,
  ERROR_OUT_OF_MEMORY,
  N_ERROR
} error_e;

#endif
