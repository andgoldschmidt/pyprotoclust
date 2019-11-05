# distutils: language = c++

from pychain cimport Chain

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PyChain:
    cdef Chain c_chain  # Hold a C++ instance which we're wrapping

    def __cinit__(self, int size):
        self.c_chain = Chain(size)

    def grow_rnn(self):
        self.c_chain.grow_chain()

    def remove_rnn(self, int rnn1, int rnn2, int iteration):
        self.c_chain.merge_indicies(rnn1, rnn2, iteration)
        self.c_chain.trim_chain()

    def initialize_distances(self, init_distance):
        n = len(init_distance)
        for i in range(n):
            for j in range(i):
                self.update_distance(i,j,init_distance[i,j])

    def update_distance(self, int index1, int index2, double distance):
        self.c_chain.set_distance(index1, index2, distance)        

    def get_rnn(self):
        return self.c_chain.chain_end_2(), self.c_chain.chain_end_1()