#include "epi_api.h"

#include "common.h"
#include "disease.h"
#include "population.h"

typedef struct epi_model_s {
  // Single population, for now.
  // TODO: implement hierarchical model for multiple populations and transport.
  size_t day;
  epi_scenario_t scenario;
  disease_t *disease;
  pop_t *population;
} epi_model_t;

epi_error_e epi_construct_model(epi_model *out,
  const epi_scenario_t *scenario, const char *dis_fname,
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
  memcpy(&(model->scenario), scenario, sizeof(epi_scenario_t));
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
  epi_error_e err;
  err = create_disease_from_file(&(model->disease), dis_fname);
  if (err != EPI_ERROR_SUCCESS) {
    free(model);
    return err;
  }

  // Read population data file
  err = create_pop_from_file(&(model->population), pop_fname);
  if (err != EPI_ERROR_SUCCESS) {
    free_disease(&(model->disease));
    free(model);
    return err;
  }

  *out = (epi_model)model;
  return EPI_ERROR_SUCCESS;
}

epi_error_e epi_free_model(epi_model *model) {
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
