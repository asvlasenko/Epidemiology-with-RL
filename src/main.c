#include "epi_api.h"

#include "disease.h"
#include "population.h"

#define INITIAL_INFECTIONS 10
#define INITIAL_POPULATION 300000000

int main(int argc, char **argv) {
  epi_error_e err;

  printf("Initializing model\n");
  disease_t *disease;
  err = create_disease_from_file(&disease, "./dat/disease.dat");
  assert(err == EPI_ERROR_SUCCESS);
  printf("  success!\n");

  printf("Creating test population\n");
  pop_t *test_pop;
  err = create_pop(&test_pop, INITIAL_POPULATION, disease->max_duration);
  assert(err == EPI_ERROR_SUCCESS);
  printf("  success!\n");

  // TODO: temporary hospital bed setup to model United States
  // 2.8 hospital beds per 1000 people, but 2/3 are used at any given time
  add_hosp_capacity(test_pop, 280000);

  err = infect_pop(test_pop, INITIAL_INFECTIONS);
  assert(err == 0);
  assert(test_pop->n_total_active[0] == INITIAL_INFECTIONS);

  // TEST: temporarily impose social distancing policies based on filled
  // hospital capacity and / or number of deaths
  uint64 last_dead = 0;
  float hload;

  // Keep track for how many days these policies were in effect
  size_t dist = 0;
  size_t home = 0;

  // Keep track of total economic loss
  float p_loss = 0.f;

  printf("Running population time evolution test\n");
  for (size_t i = 0; i < 700; i++) {

    // Control measure test hack
    uint64 n_deaths = test_pop->n_dead - last_dead;
    last_dead += n_deaths;
    hload = hospital_load(test_pop); // 0.2, 0.3, 0.8
    test_pop->policy.dist_home_symp = (hload > 0.00);
    test_pop->policy.dist_recommend = (hload > 0.32);
    test_pop->policy.dist_home_all = (hload > 0.82);

    dist += test_pop->policy.dist_recommend;
    home += test_pop->policy.dist_home_all;

    // Main step
    err = evolve_pop(test_pop, disease);
    print_pop_info(i, test_pop);
    assert(err == 0);

    // Vaccine test hack - assume rollout on day 550,
    // vaccination capability of 3% susceptible population per day
    if (i >= 550) {
      uint64 n_vac = (uint64)(test_pop->n_susceptible * 0.03f);
      test_pop->n_susceptible -= n_vac;
      test_pop->n_vaccinated += n_vac;
    }

    p_loss += productivity_loss(test_pop);
  }
  printf("\n%I64u, %I64u, %i, %i, %i\n",
    dist,
    home,
    (int)(p_loss / 1e9),
    (int)(0.008 * test_pop->n_dead),
    (int)(p_loss / 1e9 + 0.008 * test_pop->n_dead)
  );
  printf("  success!\n");

  err = free_pop(&test_pop);
  assert(!err);
  assert(test_pop == NULL);

  err = free_disease(&disease);
  assert(!err);
  assert(disease == NULL);
}
