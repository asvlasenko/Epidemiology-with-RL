#ifndef __EPI_API_H__
#define __EPI_API_H__

// Interface for setting up model, running it, and getting output
#include <stdbool.h>
#include <stdlib.h>

#include "error.h"

struct model_s;

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
error_e construct_model(struct model_s *model, const scenario_t *scenario,
  const char *dis_file, const char *pop_file);

// Free resources associated with a model.  Sets model pointer to NULL.
error_e free_model(struct model_s **model);

// Observable model output for each step - this is visible to the "player"
/*
typedef struct observable_output_s {
  // ***TODO***
} observable_output_t;
*/

// True model output for each step - this is not visible to the "player", but
// used to record the history and to determine the score
/*
typedef struct hidden_output_s {

} hidden_output_t;
*/

// TODO: move policy information from policy.h into here, since policy is
// set by the player and is therefore part of the interface

#endif
