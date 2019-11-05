# distutils: language = c++

from pylinkage cimport Linkage

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PyLinkage:
    cdef Linkage c_linkage  # Hold a C++ instance which we're wrapping

    def __cinit__(self, int size):
        self.c_linkage = Linkage(size)

    def _initialize_vectors(self, G, H):
        for ig in G: 
            self.c_linkage.add_to_G(ig)
        for ih in H:
            self.c_linkage.add_to_H(ih)      

    def minimax_linkage(self, G, H):
        self._initialize_vectors(G, H)

        self.c_linkage.minimax_linkage()

        self.c_linkage.clear_GH()

        dist = self.c_linkage.get_minimax_distance()
        center = self.c_linkage.get_minimax_center() 
        return dist,center

    def initialize_distances(self, init_distances):
        n = len(init_distances)
        for i in range(n):
            for j in range(i): # Defaults to 0 (so skip diagonals)
                self.set_distance(i,j,init_distances[i,j])

    def set_distance(self, int x, int y, double distance):
        self.c_linkage.set_distance(x,y,distance)