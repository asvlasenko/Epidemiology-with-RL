#ifndef __POPULATION_H__
#define __POPULATION_H__

#include "common.h"
#include "disease.h"
#include "policy.h"

// Population structure
#define N_POP_ARRAY_FIELDS 4
typedef struct {
  // Disease control policy in place for this population
  policy_t policy;

  // ECONOMIC MODEL
  // TODO: refactor and move to different data structure
  // Average production per person per day, in dollars
  float daily_production;

  // Fraction of people who have critical jobs, and therefore are not
  // subject to stay-at-home orders
  float f_critical_jobs;

  // Productivity impact of mild illness, without quarantine
  float prod_symp;

  // Productivity impact of social distancing, for non-critical jobs
  float prod_dist;

  // Productivity impact of home quarantine, for non-critical jobs
  float prod_home;

  // Baseline contact rates for various types of situations
  float cr_normal;
  float cr_home;
  float cr_hospital;

  uint64 n_total;
  uint64 n_susceptible;
  uint64 n_infected;
  uint64 n_recovered;
  uint64 n_vaccinated;
  uint64 n_dead;

  // Active disease phases are binned by day post infection
  size_t max_duration;

  uint64 *n_total_active;
  uint64 *n_asymptomatic;
  uint64 *n_symptomatic;
  uint64 *n_critical;

  // TODO: hospital and monitoring model
  uint64 n_hospital_beds;   // Reserve hospital capacity

  // uint64 *n_known_active;
  // uint64 n_positive_tests;
  // uint64 n_negative_tests;
  // uint64 n_known_recovered;

} pop_t;

// Constructs and fills out population information.
// Population starts out uninfected.
// Returns pointer to the population data structure.
// This pointer should be freed with the free_pop() function.
// If construction fails, frees any partially allocated resources
// and returns a NULL pointer.
epi_error_e create_pop(pop_t **out, uint64 n_people, size_t disease_duration);

// Frees population struct and associated data.  Nulls population pointer.
// Returns 1 if disease pointer is NULL, or if its internal data is NULL.
epi_error_e free_pop(pop_t **pop);

// Infect members of the population.  If requested number of infected exceeds
// the total susceptible population, then the entire susceptible population
// becomes infected.
epi_error_e infect_pop(pop_t *pop, uint64 n_cases);

// Evolve the population forward by one day
// TODO: add an argument that encodes government policies to control
// the disease
epi_error_e evolve_pop(pop_t *pop, const disease_t *dis);

// Control measure: add hospital beds to population
epi_error_e add_hosp_capacity(pop_t *pop, uint64 n_beds);

// Print info, for debug purposes
epi_error_e print_pop_info(size_t t, const pop_t *pop);

// Calculate ratio of critical cases to hospital capacity
float hospital_load(const pop_t *pop);

// TODO: move to different file
// Calculate productivity impact per day
float productivity_loss(const pop_t *pop);

#endif
