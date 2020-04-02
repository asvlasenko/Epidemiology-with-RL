// Disease information structure
#ifndef __DISEASE_H__
#define __DISEASE_H__

#include "common.h"
#include "disease.h"

#define N_DISEASE_ARRAY_FIELDS 6
typedef struct {
  size_t max_duration;          // Maximum duration before recovery, in days
  float asymp_trans_reduction;  // How much is transmission probability reduced
                                // for asymptomatic cases?
  float false_neg_reduction;    // How much is chance of false negative reduced
                                // if symptoms are present?
  float hosp_death_reduction;   // How much is death rate reduced with
                                // hospitalization of critical cases?

  // Disease progression: probabilities per day
  float *p_transmit;    // Probability of transmission
  float *p_symptoms;    // Probability of developing symptoms
  float *p_negative;    // Probability of false negative test, if no symptoms
  float *p_recovery;    // Probability of recovery by this date, if no symptoms
  float *p_critical;    // Probability of developing a dangerous condition
  float *p_death;       // Probability of death if critical and not hospitalized

} disease_t;

// Constructs and fills out disease information from a text data file.
// Returns pointer to the disease data structure.
// This pointer should be freed with the free_disease() function.
// If construction fails, frees any partially allocated resources
// and returns a NULL pointer.
epi_error_e create_disease_from_file(disease_t **dis, const char *filename);

// Frees disease struct and associated data.  Sets disease pointer to NULL.
// Returns 1 if disease pointer is NULL, or if its internal data is NULL.
epi_error_e free_disease(disease_t **dis);


#endif
