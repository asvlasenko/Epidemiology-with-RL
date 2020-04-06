#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "epi_lib/epi_api.h"

#define INITIAL_INFECTIONS 10
#define INITIAL_POPULATION 300000000

int main(int argc, char **argv) {

  printf("Testing model constructor\n");
  EpiError err;
  EpiModel model;
  EpiScenario scenario = {0, 10, 550, -1,
    "./dat/disease.dat", "./dat/population.dat"};
  err = epi_construct_model(&model, &scenario);
  assert(err == EPI_ERROR_SUCCESS);
  printf("  success!\n");

  EpiInput input = {0};
  EpiObservable output = {0};
  while(!output.finished) {
    err = epi_model_step(model, &input);
    assert(err == EPI_ERROR_SUCCESS);
    err = epi_get_observables(&output, model);
    assert(err == EPI_ERROR_SUCCESS);
  }
  printf("end day = %I64u\n", output.day);
  printf("  success!\n");

  printf("Testing model destructor\n");
  err = epi_free_model(&model);
  assert(err == EPI_ERROR_SUCCESS);
  assert(model == NULL);
  printf("  success!\n");

}
