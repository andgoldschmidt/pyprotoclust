# distutils: language = c++

from numpy import array, dtype
from itertools import combinations
from pyprotoclust cimport Protoclust
from cython.parallel import prange
from cython import boundscheck, wraparound
@boundscheck(False)  # Deactivate bounds checking
@wraparound(False)   # Deactivate negative indexing.

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PyProtoclust:
    cdef Protoclust c_protoclust  # Hold a C++ instance which we're wrapping

    def __cinit__(self, int n):
        self.c_protoclust = Protoclust(n)

    def initialize_distances(self, const double[:,:] init_distance):
        n = init_distance.shape[0]
        cdef int [:,:] index = array(list(combinations(range(n),2)), dtype=dtype("i"))
        cdef Py_ssize_t i
        for i in range(len(index)):
            self.c_protoclust.set_distance(index[i, 1], index[i, 0],
                                           init_distance[index[i, 1], index[i, 0]])

    def compute(self):
        self.c_protoclust.compute()
    
    def compute_at(self, int i):
        self.c_protoclust.compute_index(i)

    def Z(self, int n):
        return [[self.c_protoclust.get_Z_0(i),
                 self.c_protoclust.get_Z_1(i),
                 self.c_protoclust.get_Z_2(i),
                 self.c_protoclust.get_Z_3(i)]
                for i in range(n-1)]

    def center(self, int i):
        return self.c_protoclust.get_cluster_center(i)        

    def cluster_centers(self, int n):
        return [self.center(i) for i in range(2*n-1)]
            