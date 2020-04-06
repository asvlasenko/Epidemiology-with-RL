# cepi_model.pxd
# Cython API redefinition, corresponding to epi_api.h
cdef extern from "./epi_lib/epi_api.h":

    # Opaque handle to model
    ctypedef struct _EpiModel:
        pass

    ctypedef _EpiModel* EpiModel

    # Error return values
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

    # Model parameters and data
    ctypedef struct EpiScenario:
        # Day of initial infection, negative = never
        int t_initial
        # Number of infected in first round
        size_t n_initial
        # Time of vaccine arrival, negative = never
        int t_vaccine
        # Maximum scenario duration, -1 = until disease is eradicated
        # Either day of initial infection, or max duration, must be non-negative
        int t_max
        # Disease data file name
        char *dis_fname
        # Population data file name
        char *pop_fname

    # Control measures that can be put in place
    #ctypedef struct EpiInput:
        # Moderate social distancing
    #    bool dist_recommend
        # People with symptoms required to self-isolate
    #    bool dist_home_symp
        # Home quarantine orders, except for essential tasks
    #    bool dist_home_all
        # TODO: hospital capacity expansion and testing policies

    #ctypedef unsigned long long uint64

    # Observable output from model
    # TODO: for now, the player can see the real situation. Add a testing model.
    #ctypedef struct EpiObservable:
    #    size_t day

    #    bool finished
    #    bool vaccine_available

    #    uint64 hosp_capacity

    #    uint64 n_susceptible
    #    uint64 n_infected
    #    uint64 n_critical
    #    uint64 n_recovered
    #    uint64 n_vaccinated
    #    uint64 n_dead

    # Create a single-population model from scenario description,
    # a disease data file and a population data file
    EpiError epi_construct_model(EpiModel *out, EpiScenario *sc)

    # Free resources associated with a model.  Sets model pointer to NULL.
    EpiError epi_free_model(EpiModel *out)

    # Step model forward by one day, based on measures given in input
    #EpiError epi_model_step(EpiModel model, const EpiInput *input)

    # Get observable output from model
    #EpiError epi_get_observables(EpiObservable *out, const EpiModel model)
