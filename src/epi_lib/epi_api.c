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

  epi_model_t *mptr = (epi_model_t *)model;

  if(mptr == NULL || out == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  if(mptr->population == NULL || mptr->population->n_total_active == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  out->obs.day = mptr->day;

  out->obs.current_policy = &(mptr->population->policy);
  out->obs.n_total = mptr->population->n_total;
  out->obs.n_vaccinated = mptr->population->n_vaccinated;
  out->obs.n_dead = mptr->population->n_dead;
  out->obs.hosp_capacity = mptr->population->n_hospital_beds;
  out->obs.hosp_demand = mptr->population->n_total_critical;
  out->obs.finished = mptr->finished;

  out->n_total = mptr->population->n_total;
  out->n_susceptible = mptr->population->n_susceptible;
  out->n_infected = mptr->population->n_infected;
  out->n_recovered = mptr->population->n_recovered;
  out->n_vaccinated = mptr->population->n_vaccinated;
  out->n_dead = mptr->population->n_dead;

  out->max_duration = mptr->population->max_duration;

  out->n_total_active = mptr->population->n_total_active;
  out->n_asymptomatic = mptr->population->n_asymptomatic;
  out->n_symptomatic = mptr->population->n_symptomatic;
  out->n_critical = mptr->population->n_critical;

  return EPI_ERROR_SUCCESS;
}
