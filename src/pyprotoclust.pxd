cdef extern from "protoclust.h":
    pass

cdef extern from "protoclust.h" namespace "minimax":
    cdef cppclass Protoclust:
        Protoclust() except +
        Protoclust(int) except +
        
        void set_distance(int i, int j, double distance) nogil

        void compute()
        void compute_index(int i)

        int get_Z_0(int i)
        int get_Z_1(int i)
        double get_Z_2(int i)
        int get_Z_3(int i)
        int get_cluster_center(int i)