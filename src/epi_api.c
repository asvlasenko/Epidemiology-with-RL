#include "epi_api.h"

#include "common.h"
#include "disease.h"
#include "population.h"

typedef struct epi_model_s {
  // Single population, for now.
  // TODO: implement hierarchical model for multiple populations and transport.
  size_t day;
  bool started;
  bool finished;

  epi_scenario scenario;
  disease_t *disease;
  pop_t *population;
} epi_model_t;

epi_error epi_construct_model(epi_model *out,
  const epi_scenario *scenario, const char *dis_fname,
  const char *pop_fname) {

  if (out == NULL || scenario == NULL ||
    dis_fname == NULL || pop_fname == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  epi_model_t *model = calloc(1, sizeof(epi_model_t));
  if (model == NULL) {
    return EPI_ERROR_OUT_OF_MEMORY;
  }

  // Set up scenario
  memcpy(&(model->scenario), scenario, sizeof(epi_scenario));
  // Outbreak never happens: indicated by t_initial == -1
  if (model->scenario.t_initial < 0 || model->scenario.n_initial == 0) {
    model->scenario.t_initial = -1;
  }
  // Vaccine never happens: indicated by t_vaccine == -1
  if (model->scenario.t_vaccine < 0) {
    model->scenario.t_vaccine = -1;
  }
  // Do not stop at any particular time, run until no infections remain
  if (model->scenario.t_max < 0) {
    // Must have either a start or stop time
    if (model->scenario.t_initial == -1) {
      free(model);
      return EPI_ERROR_INVALID_SCENARIO;
    }
    model->scenario.t_max = -1;
  }

  // Read disease data file
  epi_error err;
  err = create_disease_from_file(&(model->disease), dis_fname);
  if (err != EPI_ERROR_SUCCESS) {
    free(model);
    return err;
  }

  // Read population data file
  err = create_pop_from_file(&(model->population), pop_fname,
    model->disease->max_duration);
  if (err != EPI_ERROR_SUCCESS) {
    free_disease(&(model->disease));
    free(model);
    return err;
  }

  *out = (epi_model)model;
  return EPI_ERROR_SUCCESS;
}

epi_error epi_free_model(epi_model *model) {
  if (model == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  epi_model_t *ptr = (epi_model_t *)(*model);

  if (ptr == NULL) {
    return EPI_ERROR_SUCCESS;
  }

  free_disease(&(ptr->disease));
  free_pop(&(ptr->population));
  free(ptr);
  *model = (epi_model)NULL;

  return EPI_ERROR_SUCCESS;
}

epi_error epi_model_step(epi_model model, const epi_input *input) {

  epi_model_t *mptr = (epi_model_t *)model;

  if (mptr == NULL || input == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  // If model has finished, do nothing
  if (mptr->finished) {
    mptr->day++;
    return EPI_ERROR_SUCCESS;
  }

  // Apply input as current policy
  memcpy(&mptr->population->policy, input, sizeof(epi_input));

  // Check for initial infection date
  // TODO: vaccination
  if (mptr->day == mptr->scenario.t_initial) {
    PASS_ERROR(infect_pop(mptr->population, mptr->scenario.n_initial));
    mptr->started = true;
  }

  // Check if max simulation time has passed or if disease has been eradicated
  if ((mptr->started && mptr->population->n_infected == 0) ||
    mptr->day >= mptr->scenario.t_max) {

    mptr->day++;
    mptr->finished = true;
    return EPI_ERROR_SUCCESS;
  }

  // Step population forward one day
  PASS_ERROR(evolve_pop(mptr->population, mptr->disease));
  mptr->day++;

  return EPI_ERROR_SUCCESS;
}

epi_error epi_get_output(epi_output *out, epi_model model) {

  /* ... TODO ... */

  return EPI_ERROR_SUCCESS;
}
