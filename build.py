from distutils.core import Extension
from Cython.Build import cythonize

py_src = 'pyprotoclust/'
cpp_src = 'pyprotoclust/cpp/src/'
cpp_h = 'pyprotoclust/cpp/include/'

# Link to py's pxd or py's cpp files. 
sources = [py_src + 'c_protoclust.pyx',
           cpp_src + 'protoclust.cpp',
           cpp_src + 'linkage.cpp',
           cpp_src + 'chain.cpp',
           cpp_src + 'ltmatrix.cpp']

# In either case, source original (non-python) h/cpp to compile correctly.
e3 = Extension(name='pyprotoclust.c_protoclust',
               sources=sources,
               include_dirs=[cpp_h],
               extra_compile_args=['-fopenmp'],
               extra_link_args=['-fopenmp']
               )

extensions = [e3]


def build(setup_kwargs):
    setup_kwargs.update({
        'ext_modules': cythonize(extensions, language_level=3)
    })



