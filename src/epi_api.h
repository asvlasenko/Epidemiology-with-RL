#ifndef __EPI_API_H__
#define __EPI_API_H__

// Interface for setting up model, running it, and getting output
#include <stdbool.h>
#include <stdlib.h>

// Error handling
typedef enum {
  EPI_ERROR_SUCCESS = 0,
  EPI_ERROR_INVALID_ARGS,
  EPI_ERROR_FILE_NOT_FOUND,
  EPI_ERROR_OUT_OF_MEMORY,
  EPI_ERROR_UNEXPECTED_STATE,
  EPI_ERROR_MISSING_DATA,
  EPI_ERROR_UNEXPECTED_EOF,
  EPI_ERROR_INVALID_DATA,
  EPI_ERROR_INVALID_SCENARIO,
  N_EPI_ERROR
} epi_error;

#define PASS_ERROR(expr) \
  {epi_error __err__ = expr; if(__err__ != EPI_ERROR_SUCCESS) return __err__;}

// Model handle
typedef size_t epi_model;

// Scenario description
typedef struct {
  // Day of initial infection, -1 = never
  int t_initial;
  // Number of infected in first round
  size_t n_initial;
  // Time of vaccine arrival, -1 = never
  int t_vaccine;
  // How long to run the scenario, -1 = to eradication
  int t_max;
} epi_scenario;

// Create a single-population model from scenario description,
// a disease data file and a population data file
epi_error epi_construct_model(epi_model *out,
  const epi_scenario *scenario, const
  char *dis_fname, const char *pop_fname);

// Free resources associated with a model.  Sets model pointer to NULL.
epi_error epi_free_model(epi_model *out);

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

typedef struct epi_input_s {
  // Is social distancing recommended?
  bool dist_recommend;

  // Are stay-at-home orders active for people with symptoms?
  bool dist_home_symp;

  // Are stay-at-home orders active for everyone?
  bool dist_home_all;

  // Are field hospitals and improvised capacity expansion measures in place?
  bool temp_hospitals;

  // Is maximum temporary hospital capacity being expanded?
  bool temp_hospital_expansion;

} epi_input;

// Step the model forward by 1 day.
// obs_out is information visible to the player.
// hidden_out is additional information that can be logged for later
// examination.
// input is player's choices for response strategies from previous day.
epi_error model_step(epi_model *model,
  struct epi_observable_output_s *obs_out,
  struct epi_hidden_output_s *hidden_out,
  const struct epi_input_s *input);

#endif
