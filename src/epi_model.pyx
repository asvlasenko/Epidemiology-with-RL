# epi_model.pyx
# Cython wrapper class for epidemiology model

# Use static linking with a single c source file,
# due to some weirdness with Cython on MinGW vs. Linux
# distutils: sources = ./src/epi_lib/single_source.c
# distutils: include_dirs = ./epi_lib/

cimport cepi_model

cdef class EpiModel:
    cdef cepi_model.EpiModel _c_model

    def __cinit__(self, cepi_model.EpiScenario sc):
        cepi_model.epi_construct_model(&self._c_model, &sc)

    def __dealloc__(self):
        cepi_model.epi_free_model(&self._c_model)
