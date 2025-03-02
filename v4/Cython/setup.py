from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("Initial Experiments\\v4\\Cython\\minimax_bot.pyx")
)

# python setup.py build_ext --inplace