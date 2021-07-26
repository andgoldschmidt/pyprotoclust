from distutils.core import Extension


# MAC OS 
# ======
# # Change the default compiler, e.g. from clang for MAC OS.
# # import os
# # COMPILE_WITH =  'g++-9'
# os.environ['CC'] = COMPILE_WITH 
# os.environ['CXX'] = COMPILE_WITH

# # @rpath must be specified to dynamically link to the correct library
# # See also: extra_link_args in Extension
# OSX_LINK_ARGS = '-Wl,-rpath,/usr/local/opt/gcc@9/lib/gcc/9/'
# ======

# TODO: It would be nice to automatically check for OPENMP support
try:
    from Cython.Build import cythonize
except ImportError:
    print('Build.py was unable to import cython.')
    USE_CYTHON = False
    src_ext = '.cpp'
    # Attempt to fall back on
else:
    USE_CYTHON = True
    src_ext = '.pyx'

py_src = 'pyprotoclust/'
cpp_src = 'pyprotoclust/cpp/src/'
cpp_h = 'pyprotoclust/cpp/include/'

# Link to py's pxd or py's cpp files. 
sources = [py_src + 'c_protoclust' + src_ext,
           cpp_src + 'protoclust.cpp',
           cpp_src + 'linkage.cpp',
           cpp_src + 'chain.cpp',
           cpp_src + 'ltmatrix.cpp']

# In either case, source original (non-python) h/cpp to compile correctly.
e3 = Extension(name='pyprotoclust.c_protoclust',
               language = 'c++',
               sources=sources,
               include_dirs=[cpp_h],
               extra_compile_args=['-fopenmp'],
               extra_link_args=['-fopenmp'] #, OSX_LINK_ARGS]
               )

extensions = [e3]

if USE_CYTHON:
  extensions = cythonize(extensions, language_level=3)


def build(setup_kwargs):
    setup_kwargs.update({
        'ext_modules': extensions,
    })



