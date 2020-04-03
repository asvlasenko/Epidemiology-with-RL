cdef extern from "../include/epi_api.h"

ctypedef unsigned long long uint64
ctypedef unsigned long long size_t

ctypedef enum epi_error:
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

ctypedef void *epi_model

ctypedef struct epi_scenario:
  int t_initial
  size_t n_initial
  int t_vaccine
  int t_max

ctypedef struct epi_input:
  bint dist_recommend
  bint dist_home_sympt
  bint dist_home_all
  bint temp_hospitals
  bint temp_hospital_expansion

ctypedef struct epi_observable:
  size_t long long day
  epi_input *current_policy
  uint64 long long n_total
  uint64 long long n_vaccinated
  uint64 long long n_dead
  uint64 long long hosp_capacity
  uint64 long long hosp_demand
  bint finished

ctypedef struct epi_output:
  epi_observable obs

  uint64 n_total
  uint64 n_susceptible
  uint64 n_infected
  uint64 n_recovered
  uint64 n_vaccinated
  uint64 n_dead

  size_t max_duration

  const uint64 *n_total_active
  const uint64 *n_asymptomatic
  const uint64 *n_symptomatic
  const uint64 *n_critical

epi_error epi_construct_model(epi_model *out, \
const epi_scenario *scenario, const char *dis_fname, const char *pop_fname)

epi_error epi_free_model(epi_model *out)

epi_error epi_model_step(epi_model model, const epi_input *input)

epi_error epi_get_output(epi_output *out, epi_model model)
