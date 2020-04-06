# cepi_model.pxd
# Cython API definition

cdef extern from "epi_lib/epi_api.h":

    # Opaque handle to model
    ctypedef struct EpiModel:
        pass

    ctypedef enum EpiError:
        EPI_ERROR_SUCCESS
        EPI_ERROR_INVALID_ARGS
        EPI_ERROR_FILE_NOT_FOUND
        EPI_ERROR_OUT_OF_MEMORY
        EPI_ERROR_UNEXPECTED_STATE
        EPI_ERROR_MISSING_DATA
        EPI_ERROR_UNEXPECTED_EOF
        EPI_ERROR_INVALID_DATA
        EPI_ERROR_INVALID_SCENARIO
        N_EPI_ERROR

    ctypedef struct EpiInput:

        // Epidemic control strategies currently in place.
        // For now, this is on a single population basis.  Some redesign will be
        // required for the multi-population model.
        typedef struct {
          // Is social distancing recommended?
          bool dist_recommend;
          // Are stay-at-home orders active for people with symptoms?
          bool dist_home_symp;
          // Are stay-at-home orders active for everyone?
          bool dist_home_all;
          // TODO: measures below not yet implemented
          // Are field hospitals and improvised capacity expansion measures in place?
          bool temp_hospitals;
          // Is maximum temporary hospital capacity being expanded?
          bool temp_hospital_expansion;
          // TODO: testing policy
        } EpiInput;

        // Observable model output for each step - this is visible to the "player"
        // For now, this is on a population basis.  Some redesign will be required
        // for the multi-population model.

        // TODO: introduce testing model.  For now, we simply assume that the player
        // knows the number of infected, critical cases and deaths.

        typedef struct {
          size_t day;

          bool finished;
          bool vaccine_available;

          uint64 hosp_capacity;

          uint64 n_susceptible;
          uint64 n_infected;
          uint64 n_critical;
          uint64 n_recovered;
          uint64 n_vaccinated;
          uint64 n_dead;
        } EpiObservable;
