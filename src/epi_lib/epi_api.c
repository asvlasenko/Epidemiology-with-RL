#include "common.h"
#include "disease.h"
#include "population.h"

struct _EpiModel {
  // Single population, for now.
  // TODO: implement hierarchical model for multiple populations and transport.
  size_t day;
  bool started;
  bool finished;
  bool vaccine_available;

  EpiScenario scenario;
  disease_t *disease;
  pop_t *population;
};

EpiError epi_construct_model(EpiModel *out, const EpiScenario *scenario) {

  if (out == NULL || scenario == NULL ||
    scenario->dis_fname == NULL || scenario->pop_fname == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  EpiModel model = calloc(1, sizeof(struct _EpiModel));
  if (model == NULL) {
    return EPI_ERROR_OUT_OF_MEMORY;
  }

  // Set up scenario
  memcpy(&(model->scenario), scenario, sizeof(EpiScenario));
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
  EpiError err;
  err = create_disease_from_file(&(model->disease), scenario->dis_fname);
  if (err != EPI_ERROR_SUCCESS) {
    free(model);
    return err;
  }

  // Read population data file
  err = create_pop_from_file(&(model->population), scenario->pop_fname,
    model->disease->max_duration);
  if (err != EPI_ERROR_SUCCESS) {
    free_disease(&(model->disease));
    free(model);
    return err;
  }

  *out = model;
  return EPI_ERROR_SUCCESS;
}

EpiError epi_free_model(EpiModel *model) {
  if (model == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  if (*model == NULL) {
    return EPI_ERROR_SUCCESS;
  }

  free_disease(&((*model)->disease));
  free_pop(&((*model)->population));
  free(*model);
  *model = NULL;

  return EPI_ERROR_SUCCESS;
}

EpiError epi_model_step(EpiModel model, const EpiInput *input) {

  if (model == NULL || input == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  // If model has finished, do nothing
  if (model->finished) {
    model->day++;
    return EPI_ERROR_SUCCESS;
  }

  // Apply input as current policy
  memcpy(&model->population->policy, input, sizeof(EpiInput));

  // Check for initial infection date
  if (model->day == model->scenario.t_initial) {
    PASS_ERROR(infect_pop(model->population, model->scenario.n_initial));
    model->started = true;
  }

  // Check if vaccine has become available
  if (!model->vaccine_available && model->day == model->scenario.t_vaccine) {
    model->vaccine_available = true;
  }

  // Check if max simulation time has passed or if disease has been eradicated
  if ((model->started && model->population->n_infected == 0) ||
    model->day >= model->scenario.t_max) {

    model->day++;
    model->finished = true;
    return EPI_ERROR_SUCCESS;
  }

  PASS_ERROR(evolve_pop(model->population, model->disease,
    model->vaccine_available));
  model->day++;

  return EPI_ERROR_SUCCESS;
}

EpiError epi_get_observables(EpiObservable *out, const EpiModel model) {

  if(model == NULL || out == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  if(model->population == NULL || model->population->n_total_active == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  out->day = model->day;

  out->finished = model->finished;
  out->vaccine_available = model->vaccine_available;

  out->hosp_capacity = model->population->n_hospital_beds;

  out->n_susceptible = model->population->n_susceptible;
  out->n_infected = model->population->n_infected;
  out->n_critical = model->population->n_total_critical;
  out->n_recovered = model->population->n_recovered;
  out->n_vaccinated = model->population->n_vaccinated;
  out->n_dead = model->population->n_dead;

  //TODO: 8e6f is the estimated cost of one death, in dollars.
  //Replace this magic number with scenario parameter.
  out->cost_function = productivity_loss(model->population) +
    8e6f * (model->population->n_dead - model->population->n_dead_last);

  return EPI_ERROR_SUCCESS;
}
