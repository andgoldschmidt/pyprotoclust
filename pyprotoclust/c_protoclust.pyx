# distutils: language = c++

from pyprotoclust.c_protoclust cimport Protoclust

# Create a Cython extension type which holds a C++ instance as an attribute and create a bunch of forwarding methods
cdef class CyProtoclust:
    cdef Protoclust c_protoclust  # Hold a C++ instance which we're wrapping

    def __cinit__(self, int n):
        self.c_protoclust = Protoclust(n)

    def initialize_distances(self, double[:,:] init_distances):
        n = len(init_distances)
        for i in range(n):
            for j in range(i): # Defaults to 0 (so skip diagonals)
                self.c_protoclust.set_distance(i,j,init_distances[i,j])

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
            