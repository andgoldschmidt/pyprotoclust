# distutils: language = c++

from pyprotoclust.c_protoclust cimport Protoclust

# Create a Cython extension type which holds a C++ instance as an attribute and create a bunch of forwarding methods
cdef class CyProtoclust:
    cdef Protoclust c_protoclust  # Hold a C++ instance which we're wrapping

    def __cinit__(self, int n):
        self.c_protoclust = Protoclust(n)

    def initialize_distances(self, double[:,:] init_distances):
        """
        Initialize the distance matrix in the cpp code by iterating over the upper left triangle.

        Args:
            init_distances (double[:,:]): A distance matrix.
        """
        n = len(init_distances)
        for i in range(n):
            for j in range(i): # Defaults to 0 (so skip diagonals)
                self.c_protoclust.set_distance(i,j,init_distances[i,j])

    def compute(self):
        """
        Compute all of the linkages of the distance matrix.
        """
        self.c_protoclust.compute()
    
    def compute_at(self, int i):
        """
        Compute the i'th linkages of the distance matrix. Exposes the underlying loop to Python to allow for status
        bars. Indices be called in succession.

        Args:
            i (int): The index of the current active linkage.
        """
        self.c_protoclust.compute_index(i)

    def Z(self, int n):
        """
        Access the linkage matrix.
        """
        return [[self.c_protoclust.get_Z_0(i),
                 self.c_protoclust.get_Z_1(i),
                 self.c_protoclust.get_Z_2(i),
                 self.c_protoclust.get_Z_3(i)]
                for i in range(n-1)]

    def center(self, int i):
        """
        Access the prototype associated with the i'th linkage, indexed from 0 to 2*n-1.

        Args:
            i (int): The index of the linkage
        """
        return self.c_protoclust.get_cluster_center(i)

    def cluster_centers(self, int n):
        """
        Access the prototype associated with all linkages.

        Args:
            n (int): The size of the original distance matrix.
        """
        return [self.center(i) for i in range(2*n-1)]
            