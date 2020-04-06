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


        typedef enum {
          EPI_ERROR_SUCCESS,
          EPI_ERROR_INVALID_ARGS,
          EPI_ERROR_FILE_NOT_FOUND,
          EPI_ERROR_OUT_OF_MEMORY,
          EPI_ERROR_UNEXPECTED_STATE,
          EPI_ERROR_MISSING_DATA,
          EPI_ERROR_UNEXPECTED_EOF,
          EPI_ERROR_INVALID_DATA,
          EPI_ERROR_INVALID_SCENARIO,
          N_EPI_ERROR
        } epi_error;
