# distutils: language = c++

from pyprotoclust cimport Protoclust

try:
    from tqdm import tqdm_notebook, tqdm
except:
    None

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PyProtoclust:
    cdef Protoclust c_protoclust  # Hold a C++ instance which we're wrapping
    # cdef public int n # Hold a size member

    def __cinit__(self, int n):
        # self.n = n
        self.c_protoclust = Protoclust(n)

    def initialize_distances(self, init_distance):
        n = len(init_distance)
        for i in range(n):
            for j in range(i):
                self.set_distance(i,j,init_distance[i,j])

    def set_distance(self, int index1, int index2, double distance):
        self.c_protoclust.set_distance(index1, index2, distance)

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
                
    def cluster_centers(self, int n):
        return [self.c_protoclust.get_cluster_center(i)
                for i in range(n-1)]
            