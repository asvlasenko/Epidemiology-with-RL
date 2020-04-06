# epi_model.pyx
# Cython wrapper class for epidemiology model

# Use static linking with a single c source file,
# due to some weirdness with Cython on MinGW vs. Linux
# distutils: sources = ./src/epi_lib/single_source.c
# distutils: include_dirs = ./epi_lib/

cimport cepi_model

def HandleError(cepi_model.EpiError err):
    if err is cepi_model.EpiError.EPI_ERROR_SUCCESS:
        return
    if err is cepi_model.EpiError.EPI_ERROR_OUT_OF_MEMORY:
        raise MemoryError()
    # TODO: add remaining error handling cases
    raise ValueError()

cdef class EpiModel:
    cdef cepi_model.EpiModel _c_model

    def __cinit__(self, cepi_model.EpiScenario sc):
        cdef cepi_model.EpiError err
        err = cepi_model.epi_construct_model(&self._c_model, &sc)
        HandleError(err)

    def __dealloc__(self):
        cdef cepi_model.EpiError err
        err = cepi_model.epi_free_model(&self._c_model)
        HandleError(err)
