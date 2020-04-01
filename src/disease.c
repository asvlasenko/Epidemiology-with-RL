#include "disease.h"
#include "files.h"

int read_disease_params(disease_t *dis, FILE *fp);
int allocate_disease_arrays(disease_t *dis);
int read_disease_arrays(disease_t *dis, FILE *fp);

disease_t *create_disease_from_file(char *filename) {
  printf("create_disease_from_file(): %s\n", filename);
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: create_disease_from_file(): could not open %s\n", filename);
    return NULL;
  }

  printf("  creating disease object\n");
  disease_t *dis = (disease_t *)calloc(1, sizeof(disease_t));
  if (dis == NULL) {
    printf("Error: create_disease_from_file(): calloc failed\n");
    fclose(fp);
    return NULL;
  }

  printf("  reading parameters\n");
  if (read_disease_params(dis, fp)) {
    printf("Error: create_disease_from_file(): missing parameter tokens in file\n");
    fclose(fp);
    free(dis);
    return NULL;
  }

  printf("  allocating memory\n");
  if (allocate_disease_arrays(dis)) {
    printf("Error: create_disease_from_file(): could not create disease arrays\n");
    fclose(fp);
    free(dis);
    return NULL;
  }

  printf("  reading disease progression data\n");
  if (read_disease_arrays(dis, fp)) {
    printf("Error: create_disease_from_file(): could not fill disease arrays\n");
    fclose(fp);
    free_disease(&dis);
    return NULL;
  }

  fclose(fp);
  return dis;
}

int free_disease(disease_t **dis) {
  if (dis == NULL || *dis == NULL) {
    return 1;
  }

  if ((*dis)->p_transmit == NULL) {
    free(*dis);
    *dis = NULL;
    return 1;
  }

  free((*dis)->p_transmit);
  (*dis)->p_transmit = NULL;
  *dis = NULL;
  return 0;
}

int read_disease_params(disease_t *dis, FILE *fp) {
  int err = 0;

  err += read_size_token(&(dis->max_duration), fp, "MAX_DURATION");
  err += read_float_token(&(dis->asymp_trans_reduction), fp,
    "ASYMP_TRANS_REDUCTION");
  err += read_float_token(&(dis->false_neg_reduction), fp,
    "FALSE_NEG_REDUCTION");
  err += read_float_token(&(dis->hosp_death_reduction), fp,
    "HOSP_DEATH_REDUCTION");

  return err;
}

int allocate_disease_arrays(disease_t *dis) {
  size_t n = dis->max_duration;
  if (!n) {
    printf("Error: allocate_disease_arrays(): invalid disease duration\n");
    return 1;
  }

  float *ptr = (float *)malloc(N_DISEASE_ARRAY_FIELDS * n * sizeof(float));
  if (ptr == NULL) {
    printf("Error: allocate_disease_arrays(): malloc failed\n");
    return 1;
  }

  dis->p_transmit = ptr;
  dis->p_symptoms = &ptr[n];
  dis->p_negative = &ptr[2*n];
  dis->p_recovery = &ptr[3*n];
  dis->p_critical = &ptr[4*n];
  dis->p_death = &ptr[5*n];

  // Some paranoia to catch incomplete changes to disease data type
  float *ptr_last = &dis->p_death[n-1];
  assert(ptr_last - ptr == N_DISEASE_ARRAY_FIELDS * n - 1);

  return 0;
}

int read_disease_arrays(disease_t *dis, FILE *fp) {
  int err = 0;

  err += read_float_array(dis->p_transmit, dis->max_duration, fp, "P_TRANSMIT");
  err += read_float_array(dis->p_symptoms, dis->max_duration, fp, "P_SYMPTOMS");
  err += read_float_array(dis->p_negative, dis->max_duration, fp, "P_NEGATIVE");
  err += read_float_array(dis->p_recovery, dis->max_duration, fp, "P_RECOVERY");
  err += read_float_array(dis->p_critical, dis->max_duration, fp, "P_CRITICAL");
  err += read_float_array(dis->p_death, dis->max_duration, fp, "P_DEATH");

  return err;
}
