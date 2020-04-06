#include "approx_binomial.h"

// Draw a number of events from a Poisson distribution
static epi_error_e poisson_draw(uint64 *k, float rate);

// Generate a normally distributed floating point number
// with mean = 0 and variance = 1
static float rand_normal();

epi_error_e approx_dbin_draw(uint64 *nx, uint64 *ny,
  float p_x, float p_y, uint64 n) {

  if (nx == NULL || ny == NULL || p_x + p_y > 1.f || p_x < 0.f || p_y < 0.f) {
    return EPI_ERROR_INVALID_ARGS;
  }

  // First, find probability of either event X or event Y
  float p_xy = p_x + p_y;

  // Edge case: zero events expected
  if (p_xy * n == 0.f) {
    *nx = *ny = 0;
    return EPI_ERROR_SUCCESS;
  }

  // Get number of events x + events y
  uint64 nxy;

  int err;
  err = approx_bin_draw(&nxy, p_xy, n);
  if (err != EPI_ERROR_SUCCESS) {
    return err;
  }

  if (nxy == 0) {
    *nx = *ny = 0;
    return EPI_ERROR_SUCCESS;
  }

  // Get number of events x, specifically
  // Edge case: one of the probabilities is zero
  if (p_x == 0.f) {
    *nx = 0;
    *ny = nxy;
    return EPI_ERROR_SUCCESS;
  }
  if (p_y == 0.f) {
    *nx = nxy;
    *ny = 0;
    return EPI_ERROR_SUCCESS;
  }

  // Probability of x, given that either x or y occurred
  float p = p_x / (p_x + p_y);
  err = approx_bin_draw(nx, p, nxy);
  if (err) {
    return err;
  }
  *ny = nxy - *nx;
  return EPI_ERROR_SUCCESS;
}

// Outcome probability cutoff for approximating binomial distribution
// as a Poisson distribution
#define POISSON_CUTOFF 0.1

// Mean deviation to expectation value ratio cutoff for approximating binomial
// distribution as a Gaussian distribution
#define GAUSSIAN_CUTOFF 0.5

epi_error_e approx_bin_draw(uint64 *k, float p, uint64 n) {
  if (k == NULL || p < 0.f || p > 1.f) {
    return EPI_ERROR_INVALID_ARGS;
  }

  // Edge case, p == 0 or n == 0
  if (p * n == 0.f) {
    *k = 0;
    return EPI_ERROR_SUCCESS;
  }

  // Work with smaller probability
  bool swap = false;
  if (p > 0.5f) {
    p = 1.f - p;
    // Edge case, p == 1 (before swap)
    // <= used instead of == in case of rounding error in last step
    if (p * n <= 0.f) {
      *k = n;
      return EPI_ERROR_SUCCESS;
    }
    swap = true;
  }

  uint64 result;

  // p << 1: use Poisson sampling, retry if we end up with k > n (unlikely)
  if (p <= POISSON_CUTOFF) {
    do {
      if (poisson_draw(&result, p * n)) {
        return EPI_ERROR_UNEXPECTED_STATE;
      }
    } while (result > n);
  } else {
    float ev = p * n;
    float std = (float)sqrt(ev * (1.f - p));

    // standard deviation of k << <k>: use Gaussian sampling, retry
    // if we end up with k < 0 or k > n
    if (std <= ev * GAUSSIAN_CUTOFF) {
      float z;
      for(;;) {
        z = ev + std * rand_normal();
        if (z < 0.f) {
          continue;
        }
        result = (uint64)z;
        if (result <= n) {
          break;
        }
      }
    }
    // If we are here, n is reasonably small:
    // we can show n < 1/(GAUSSIAN_CUTOFF^2 * POISSON_CUTOFF)
    // We could use clever algorithms like BTPE to squeeze out some extra
    // performance, but for now let's just use Monte Carlo
    else {
      result = 0;
      for (size_t i = 0; i < n; i++) {
        float z = (float)rand() / (float)RAND_MAX;
        if (z < p) {
          result++;
        }
      }
    }
  }

  *k = swap ? n - result : result;
  return EPI_ERROR_SUCCESS;
}

// Max steps in accept-reject method before we assume there is an error
#define POISSON_MAX_STEPS 1024

static epi_error_e poisson_draw(uint64 *k, float rate) {
  if (k == NULL || rate < 0.f) {
    return EPI_ERROR_INVALID_ARGS;
  }
  if (rate == 0.f) {
    *k = 0;
    return EPI_ERROR_SUCCESS;
  }

  // For large rates, use accept-reject method based on normal approximation
  // TODO: add reference (it's in a 1979 paper somewhere)
  if (rate > 30.f) {
    float c = 0.767f - 3.36f / rate;
    float b = (float)M_PI / (float)sqrt(3.f * rate);
    float a = b * rate;
    float z = (float)log(c/b) - rate;

    for(size_t i = 0; i < POISSON_MAX_STEPS; i++) {
      float u = (float)rand() / (float)RAND_MAX;
      float v = 1.f - u;

      // Avoid floating point errors like division by zero or log(0)
      if (u == 0.f) {
        u = 1.f / (float)RAND_MAX;
        v -= u;
      } else if (v == 0.f) {
        v = 1.f / (float)RAND_MAX;
        u -= v;
      }

      float x = (a - (float)log(v/u))/b;

      // Reject negative results
      if (x + 0.5f <= 0.f) {
        continue;
      }
      uint64 n = (uint64)(x + 0.5f);

      // Main rejection step
      float w = (float)rand() / (float)RAND_MAX;
      if (w == 0.f) {
        w = 1.f / (float)RAND_MAX;
      }
      float y = a - b * x;
      float d = 1.f + (float)exp(y);
      d *= d;
      if (y + (float)log(w/d) <=
        z + n*(float)log(rate) - (float)lgamma((float)n + 1.f)) {
        *k = n;
        return 0;
      }
    }
  }

  // If we got here, either the rate is low, or the accept-reject algorithm
  // failed (which should be astronomically improbable)

  float z = (float)exp(-rate);
  // Rate is too high and the accept-reject algorithm failed,
  // this should never happen
  if (z == 0.f) {
    return EPI_ERROR_UNEXPECTED_STATE;
  }

  // Knuth algorithm
  uint64 n = 0;
  float p = 1.f;
  do {
    n++;
    float u = (float)rand() / (float)RAND_MAX;
    p *= u;
  } while (p > z);
  *k = n - 1;
  return EPI_ERROR_SUCCESS;
}

// Marsaglia's algorithm for generating normally distributed random numbers
static float rand_normal() {
  float x, y, r2;
  do {
    x = (float)rand() / (float)RAND_MAX;
    y = (float)rand() / (float)RAND_MAX;
    x = 2.f * x - 1.f;
    y = 2.f * y - 1.f;
    r2 = x * x + y * y;
  } while(r2 >= 1.f || r2 <= 0.f);

  return x * (float)sqrt(-2.f * log(r2) / r2);
}
