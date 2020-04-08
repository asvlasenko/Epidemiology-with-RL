#include "disease.h"
#include "files.h"

static EpiError read_disease_params(Disease *dis, FILE *fp);
static EpiError allocate_disease_arrays(Disease *dis);
static EpiError read_disease_arrays(Disease *dis, FILE *fp);

EpiError create_disease_from_file(Disease **out, const char *filename) {
  if (out == NULL || filename == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return EPI_ERROR_FILE_NOT_FOUND;
  }

  Disease *dis = (Disease *)calloc(1, sizeof(Disease));
  if (dis == NULL) {
    fclose(fp);
    return EPI_ERROR_OUT_OF_MEMORY;
  }

  EpiError err = read_disease_params(dis, fp);
  if (err != EPI_ERROR_SUCCESS) {
    fclose(fp);
    free(dis);
    return err;
  }

  err = allocate_disease_arrays(dis);
  if (err != EPI_ERROR_SUCCESS) {
    fclose(fp);
    free(dis);
    return err;
  }

  err = read_disease_arrays(dis, fp);
  if (err != EPI_ERROR_SUCCESS) {
    fclose(fp);
    free_disease(&dis);
    return err;
  }

  fclose(fp);

  *out = dis;
  return EPI_ERROR_SUCCESS;
}

EpiError free_disease(Disease **dis) {
  if (dis == NULL) {
    return EPI_ERROR_INVALID_ARGS;
  }

  // Null pointer OK
  if (*dis == NULL) {
    return EPI_ERROR_SUCCESS;
  }

  if ((*dis)->p_transmit == NULL) {
    free(*dis);
    *dis = NULL;
    return EPI_ERROR_SUCCESS;
  }

  free((*dis)->p_transmit);
  (*dis)->p_transmit = NULL;
  free(*dis);
  *dis = NULL;

  return EPI_ERROR_SUCCESS;
}

static EpiError read_disease_params(Disease *dis, FILE *fp) {
  PASS_ERROR(read_size_token(&(dis->max_duration), fp, "MAX_DURATION"));
  PASS_ERROR(read_float_token(&(dis->asymp_trans_reduction), fp,
    "ASYMP_TRANS_REDUCTION"));
  PASS_ERROR(read_float_token(&(dis->false_neg_reduction), fp,
    "FALSE_NEG_REDUCTION"));
  PASS_ERROR(read_float_token(&(dis->hosp_death_reduction), fp,
    "HOSP_DEATH_REDUCTION"));
  return EPI_ERROR_SUCCESS;
}

static EpiError allocate_disease_arrays(Disease *dis) {
  if (dis == NULL || !dis->max_duration) {
    return EPI_ERROR_INVALID_ARGS;
  }

  size_t n = dis->max_duration;

  float *ptr = (float *)calloc(N_DISEASE_ARRAY_FIELDS * n, sizeof(float));
  if (ptr == NULL) {
    return EPI_ERROR_OUT_OF_MEMORY;
  }

  dis->p_transmit = ptr;
  dis->p_symptoms = &ptr[n];
  dis->p_negative = &ptr[2*n];
  dis->p_recovery = &ptr[3*n];
  dis->p_critical = &ptr[4*n];
  dis->p_death = &ptr[5*n];

  return EPI_ERROR_SUCCESS;
}

static EpiError read_disease_arrays(Disease *dis, FILE *fp) {
  PASS_ERROR(read_float_array(dis->p_transmit,
    dis->max_duration, fp, "P_TRANSMIT"));
  PASS_ERROR(read_float_array(dis->p_symptoms,
    dis->max_duration, fp, "P_SYMPTOMS"));
  PASS_ERROR(read_float_array(dis->p_negative,
    dis->max_duration, fp, "P_NEGATIVE"));
  PASS_ERROR(read_float_array(dis->p_recovery,
    dis->max_duration, fp, "P_RECOVERY"));
  PASS_ERROR(read_float_array(dis->p_critical,
    dis->max_duration, fp, "P_CRITICAL"));
  PASS_ERROR(read_float_array(dis->p_death, dis->max_duration, fp, "P_DEATH"));

  return EPI_ERROR_SUCCESS;
}
