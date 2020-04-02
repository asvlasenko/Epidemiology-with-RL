#ifndef __EPI_API_H__
#define __EPI_API_H__

// Interface for setting up model, running it, and getting output
#include <stdbool.h>
#include <stdlib.h>

#include "error.h"

struct epi_model_s;

// Scenario description
typedef struct scenario_s {
  // Day of initial infection, -1 = never
  int t_initial;
  // Number of infected in first round
  size_t n_initial;
  // Time of vaccine arrival, -1 = never
  int t_vaccine;
  // How long to run the scenario, -1 = to eradication
  int t_max;
} scenario_t;

// Create a single-population model from scenario description and 2 data files.
// First file describes the disease, second one describes the population.
error_e epi_construct_model(struct epi_model_s *model,
  const scenario_t *scenario, const char *dis_file, const char *pop_file);

// Free resources associated with a model.  Sets model pointer to NULL.
error_e epi_free_model(struct epi_model_s **model);

struct epi_observable_output_s;
// Observable model output for each step - this is visible to the "player"
/*
typedef struct observable_output_s {
  // ***TODO***
} observable_output_t;
*/

// True model output for each step - this is not visible to the "player", but
// used to record the history and to determine the score
struct epi_hidden_output_s;
/*
typedef struct hidden_output_s {
  // ***TODO***
} hidden_output_t;
*/

// TODO: integrate with policy.h, as input describes policy at given time
struct epi_input_s;
/*
typedef struct input_s {
  // ***TODO***
} input_t;
*/

error_e model_step(struct epi_model_s *model,
  struct epi_observable_output_s *obs_out,
  struct epi_hidden_output_s *hidden_out,
  const struct epi_input_s *input);

#endif
