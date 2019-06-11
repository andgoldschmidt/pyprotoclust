# distutils: sources = distance.cpp
# distutils: language = c++

from pydistance cimport Distance
import numpy as np

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.

cdef class PyDistance:
    cdef Distance c_distance  # Hold a C++ instance which we're wrapping

    def __cinit__(self, data):
        n, d = data.shape
        self.c_distance = Distance(n, d)

        for row, vec in enumerate(data):
            for col, entry in enumerate(vec):
                self.c_distance.load_entry(entry, row, col)
        self._compute_distance_matrix()

    def __len__(self):
        return self.c_distance.size()

    def __getitem__(self, key):
        try:
            # Always a tuple of ints, no slicing.
            k1, k2 = key
            return  self.c_distance.lookup_distance(k1, k2)
        except:
            # Failure to lookup.
            raise IndexError("Key must be a pair of integers.")

    def view_data(self, i, j):
        return self.c_distance.view_data(i, j)

    def _compute_distance_matrix(self):
        self.c_distance.compute_distances()

    def asarray(self):
        a = np.empty((len(self),len(self)))
        for i in range(len(self)):
            for j in range(len(self)):
                a[i,j] = self[i,j]
        return a
        

