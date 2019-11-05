cdef extern from "chain.h":
    pass

cdef extern from "chain.h" namespace "minimax":
    cdef cppclass Chain:
        Chain() except +
        Chain(int) except +
        void grow_chain()
        void set_distance(int x, int y, float distance)
        void merge_indicies(int r1, int r2, int iteration)
        void trim_chain()
        int chain_end_1()
        int chain_end_2()