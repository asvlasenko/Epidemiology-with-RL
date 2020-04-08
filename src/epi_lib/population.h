#ifndef __POPULATION_H__
#define __POPULATION_H__

#include "common.h"
#include "disease.h"

// Population structure
#define N_POP_ARRAY_FIELDS 4
typedef struct {
  // Disease control policy in place for this population
  EpiInput policy;

  // ECONOMIC MODEL
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
  uint64 n_total_critical;
  uint64 n_recovered;
  uint64 n_vaccinated;
  uint64 n_dead_last;
  uint64 n_dead;

  // Fraction of population that can be vaccinated each day
  float daily_vaccination_capacity;

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

} Population;

// Create population from data file
EpiError create_pop_from_file(Population **out, const char *fname,
  size_t disease_duration);

// Frees population struct and associated data.  Nulls population pointer.
EpiError free_pop(Population **pop);

// Infect members of the population.  If requested number of infected exceeds
// the total susceptible population, then the entire susceptible population
// becomes infected.
EpiError infect_pop(Population *pop, uint64 n_cases);

// Evolve the population forward by one day
// TODO: add an argument that encodes government policies to control
// the disease
EpiError evolve_pop(Population *pop, const Disease *dis, bool vaccine);

// Control measure: add hospital beds to population
EpiError add_hosp_capacity(Population *pop, uint64 n_beds);

// Print info, for debug purposes
EpiError print_pop_info(size_t t, const Population *pop);

// Calculate ratio of critical cases to hospital capacity
float hospital_load(const Population *pop);

// TODO: move to different file
// Calculate productivity impact per day
float productivity_loss(const Population *pop);

#endif
