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
} EpiError;

// Opaque handle for model
typedef struct _EpiModel* EpiModel;

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
  // Name of disease data file
  char *dis_fname;
  // Name of population data file
  char *pop_fname;
} EpiScenario;

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
  //bool temp_hospitals;
  // Is maximum temporary hospital capacity being expanded?
  //bool temp_hospital_expansion;
  // TODO: testing policy
} EpiInput;

// Observable model output for each step - this is visible to the "player"
// For now, this is on a population basis.  Some redesign will be required
// for the multi-population model.

// TODO: introduce testing model.  For now, we simply assume that the player
// knows the number of infected, critical cases and deaths.

typedef struct {
  size_t day;

  bool finished;
  bool vaccine_available;

  uint64 hosp_capacity;

  uint64 n_susceptible;
  uint64 n_infected;
  uint64 n_critical;
  uint64 n_recovered;
  uint64 n_vaccinated;
  uint64 n_dead;
} EpiObservable;

// Create a single-population model from scenario description,
// a disease data file and a population data file
EpiError epi_construct_model(EpiModel *out, const EpiScenario *scenario);

// Free resources associated with a model.  Sets model pointer to NULL.
EpiError epi_free_model(EpiModel *out);

// Step model forward by one day, based on measures given in input
EpiError epi_model_step(EpiModel model, const EpiInput *input);

// Get observable output from model
EpiError epi_get_observables(EpiObservable *out, const EpiModel model);

#endif
