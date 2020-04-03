#ifndef __EPI_API_H__
#define __EPI_API_H__

// Interface for setting up model, running it, and getting output

#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
typedef uint64_t uint64;

// Error handling
typedef enum {
  EPI_ERROR_SUCCESS,
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

// Model handle
typedef struct epi_model_s epi_model_t;
typedef epi_model_t* EpiModel;

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
} EpiScenario;

// Create a single-population model from scenario description,
// a disease data file and a population data file
epi_error epi_construct_model(EpiModel *out,
  const EpiScenario *scenario,
  const char *dis_fname, const char *pop_fname);

// Free resources associated with a model.  Sets model pointer to NULL.
epi_error epi_free_model(EpiModel *out);

// Epidemic control strategies currently in place.
// For now, this is on a single population basis.  Some redesign will be
// required for the multi-population model.
typedef struct {
  // Is social distancing recommended?
  bool dist_recommend;
  // Are stay-at-home orders active for people with symptoms?
  bool dist_home_symp;
  // Are stay-at-home orders active for everyone?
  bool dist_home_all;
  // TODO: measures below not yet implemented
  // Are field hospitals and improvised capacity expansion measures in place?
  bool temp_hospitals;
  // Is maximum temporary hospital capacity being expanded?
  bool temp_hospital_expansion;
  // TODO: testing policy
} EpiInput;

// Observable model output for each step - this is visible to the "player"
// For now, this is on a population basis.  Some redesign will be required
// for the multi-population model.

// TODO: for now, this is very simple: we assume that the number of deaths,
// total hospital capacity, and demand for hospital capacity is known.
// This is the situation if no diagnostic test is available.

// In the future, add a testing model, with inputs (testing policy),
// outputs (number of tests performed and results), separate sub-populations
// for known positives and negatives, and policies based on test results.

typedef struct {
  size_t day;
  const EpiInput *current_policy;

  uint64 n_total;
  uint64 n_vaccinated;
  uint64 n_dead;

  // TODO: at this point, this is reserve capacity.  Model total capacity with
  // background typically around 2/3 of total, but variable
  uint64 hosp_capacity;

  // TODO: background noise on hosp_demand from unrelated causes
  uint64 hosp_demand;

  // TODO: estimated number of people who called in sick to work, subject
  // to background noise
  // uint64 n_sick;

  // Is the simulation finished?
  bool finished;

} EpiObservable;

// True model output for each step - this is not visible to the "player", but
// is used to determine the score and construct simulation logs
typedef struct {
  EpiObservable obs;

  uint64 n_total;
  uint64 n_susceptible;
  uint64 n_infected;
  uint64 n_recovered;
  uint64 n_vaccinated;
  uint64 n_dead;

  size_t max_duration;

  const uint64 *n_total_active;
  const uint64 *n_asymptomatic;
  const uint64 *n_symptomatic;
  const uint64 *n_critical;

} EpiOutput;

// Step the model forward by 1 day.
// obs_out is information visible to the player.
// hidden_out is additional information that can be logged for later
// examination.
// input is player's choices for response strategies from previous day.
epi_error epi_model_step(EpiModel model, const EpiInput *input);

epi_error epi_get_output(EpiOutput *out, const EpiModel model);

#endif
