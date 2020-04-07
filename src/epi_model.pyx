# epi_model.pyx
# Cython wrapper class for epidemiology model

# Use static linking with a single c source file,
# due to some weirdness with Cython on MinGW vs. Linux
# distutils: sources = ./src/epi_lib/single_source.c
# distutils: include_dirs = ./epi_lib/

cdef extern from "stdbool.h":
    ctypedef bint bool

cimport cepi_model

def HandleError(cepi_model.EpiError err):
    if err is cepi_model.EpiError.EPI_ERROR_SUCCESS:
        return
    if err is cepi_model.EpiError.EPI_ERROR_OUT_OF_MEMORY:
        raise MemoryError()
    # TODO: add remaining error handling cases
    raise ValueError()

class EpiScenario:
    t_initial = 0
    n_initial = 10
    t_vaccine = 550
    t_max = -1
    dis_fname = "./dat/disease.dat"
    pop_fname = "./dat/population.dat"

class EpiInput:
    dist_recommend = False
    dist_home_symp = False
    dist_home_all = False

class EpiObservables:
    day = 0
    finished = False
    vaccine_available = False
    hosp_capacity = 0
    n_susceptible = 0
    n_infected = 0
    n_critical = 0
    n_recovered = 0
    n_vaccinated = 0
    n_dead = 0

    def __cinit__(self, cepi_model.EpiObservable obs):
        self.day = obs.day
        self.finished = obs.finished
        self.vaccine_available = obs.vaccine_available
        self.hosp_capacity = obs.hosp_capacity
        self.n_susceptible = obs.n_susceptible
        self.n_infected = obs.n_infected
        self.n_critical = obs.n_critical
        self.n_recovered = obs.n_recovered
        self.n_vaccinated = obs.n_vaccinated
        self.n_dead = obs.n_dead

cdef class EpiModel:
    cdef cepi_model.EpiModel _c_model

    def __cinit__(self, scenario):
        cdef cepi_model.EpiScenario sc

        sc.t_initial = scenario.t_initial
        sc.t_initial = scenario.n_initial
        sc.t_initial = scenario.t_vaccine
        sc.t_initial = scenario.t_max
        sc.t_initial = scenario.dis_fname
        sc.t_initial = scenario.pop_fname

        cdef cepi_model.EpiError err
        err = cepi_model.epi_construct_model(&self._c_model, &sc)
        HandleError(err)

    def __dealloc__(self):
        cdef cepi_model.EpiError err
        err = cepi_model.epi_free_model(&self._c_model)
        HandleError(err)

    def step(self, input):
        cdef cepi_model.EpiInput inp

        inp.dist_recommend = input.dist_recommend
        inp.home_symp = input.home_symp
        inp.home_all = input.home_all

        cdef cepi_model.EpiError err
        err = cepi_model.epi_model_step(self._c_model, &inp)
        HandleError(err)

    def get_observables(self):
        cdef cepi_model.EpiError err
        cdef cepi_model.EpiObservable output
        err = cepi_model.epi_get_observables(&output, self._c_model)
        HandleError(err)

        out = EpiObservables(output)

        return out
