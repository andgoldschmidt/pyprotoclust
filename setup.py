from distutils.core import setup
from distutils.core import Extension

py_src = 'src/'
cpp_src = 'cpp/src/'
cpp_h = 'cpp/include/'

# If False, use cythonized cpps to compile.
# TODO: How to set?
USE_CYTHON = True 

ext = '.pyx' if USE_CYTHON else '.cpp'

# Link to py's pxd or py's cpp files. 
sources = [py_src + 'pyprotoclust' + ext, cpp_src + 'protoclust.cpp',
           cpp_src + 'linkage.cpp', cpp_src + 'chain.cpp', cpp_src + 'ltmatrix.cpp']
# In either case, source original (non-python) h/cpp to compile correctly.
e3 = Extension('pyprotoclust', 
               sources=sources,
               include_dirs=[cpp_h],
               extra_compile_args=['-fopenmp'],
               extra_link_args=['-fopenmp']
              )
extensions = [e3]

if USE_CYTHON:
    from Cython.Build import cythonize
    extensions = cythonize(extensions, compiler_directives={'language_level' : "3"})

setup(
    ext_modules = extensions
)