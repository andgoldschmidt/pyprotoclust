from distutils.core import setup
from distutils.core import Extension

py_src = 'src/'
cpp_src = 'cpp/src/'
cpp_h = 'cpp/include/'

USE_CYTHON = False # TODO: How to use this variable?

ext = '.pyx' if USE_CYTHON else '.cpp'

# Link to py pxd or compile py cpp files. In either case, source original (non-python) h/cpp.
e1 = Extension('pychain', [py_src + 'pychain' + ext, cpp_src + 'chain.cpp'], include_dirs=[cpp_h])
e2 = Extension('pylinkage', [py_src + 'pylinkage' + ext, cpp_src + 'linkage.cpp'], include_dirs=[cpp_h])
extensions = [e1,e2]

if USE_CYTHON:
    from Cython.Build import cythonize
    extensions = cythonize(extensions)

setup(
    ext_modules = extensions
)