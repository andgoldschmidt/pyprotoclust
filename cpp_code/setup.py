from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("pychain.pyx")
)

setup(
    ext_modules= cythonize("pylinkage.pyx")
)