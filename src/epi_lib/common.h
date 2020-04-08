#ifndef __COMMON_H__
#define __COMMON_H__

// When compiling on Windows, keep Microsoft from warning you to use their
// libraries instead of standard ones
#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926536
#endif

#include "epi_api.h"

#define PASS_ERROR(expr) \
  {EpiError __err__ = expr; if(__err__ != EPI_ERROR_SUCCESS) return __err__;}

#endif
