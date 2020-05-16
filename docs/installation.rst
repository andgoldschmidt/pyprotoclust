Install from source
===================

For most users, the default installation using pip should be sufficient.

To build the package from `source on github <https://github.com/andgoldschmidt/pyprotoclust>`_
use `Poetry <https://python-poetry.org/>`_. Follow the installation instructions for Poetry
and use

.. code-block:: bash

	$ poetry shell
	$ poetry install
	$ poetry build

If you are building with cython, use

.. code-block:: bash

	poetry install -E cython

For Mac OS users, make sure to rebuild with cython.

If you are building the Sphinx documentation or want to be sure all packages are installed to run the notebooks,

.. code-block:: bash

	poetry install -E docs


Mac OS
------

On Mac OS, the default compiler, *clang*, does not have openMP support. The package uses openMP as compiler arguments in this package's *build.py* script as

.. code-block:: python

   extra_compile_args=['-fopenmp'],
   extra_link_args=['-fopenmp']

These can be removed. Alternatively, to take advantage of multithreading install a *g++* compiler using homebrew

.. code-block:: bash

	$ brew install gcc

After installation, check the various g++ on the system, e.g. *g++ --version*, to find the one installed from homebrew. For example,

.. code-block:: bash

	$  g++-9 --version  
	>> g++-9 (Homebrew GCC 9.2.0_3) 9.2.0  
	>> ...

is an example of a desired compiler. Now set some environment variables for this compiler to be used by the build. Go to build.py and uncomment 

.. code-block:: python

	# Change the default compiler, e.g. from clang for MAC OS.
    import os
    COMPILE_WITH =  "g++-9"
    os.environ["CC"] = COMPILE_WITH 
    os.environ["CXX"] = COMPILE_WITH
	
where *COMPILE_WITH* is replaced with the compiler you just installed.
