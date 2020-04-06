from setuptools import Extension, setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize([Extension("epi_model", ["src/epi_model.pyx"])])
)
