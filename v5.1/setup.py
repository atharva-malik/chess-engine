from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("v5\\chess.pyx")
)

# python setup.py build_ext --inplace