#ifndef __APPROX_BINOMIAL_H__
#define __APPROX_BINOMIAL_H__
// Draw from a binomial distribution, using mean + variance approximation

#include "common.h"

// Approximate double draw from a binomial distribution.
// Outcomes x and y are mutually exclusive, having probabilities p_x and p_y
// that must add up to 1.0 or less.  There is also a possibility that neither
// x nor y happens.
// Given n experiments, generate a random pair {nx = number of outcomes x,
// ny = number of outcomes y}, with an approximately correct probability.
// In the disease model, this is used to determine how many patients from a
// population recover, have their condition worsen, or neither.
int approx_dbin_draw(uint64 *nx, uint64 *ny, float p_x, float p_y, uint64 n);

// Approximate draw from a binomial distribution.
// Determine the number of outcomes k, having probability per experiment p,
// from n experiments.
int approx_bin_draw(uint64 *k, float p, uint64 n);

#endif
