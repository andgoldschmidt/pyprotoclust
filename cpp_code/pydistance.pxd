cdef extern from "distance.h":
    pass

cdef extern from "distance.h" namespace "minimax":
    cdef cppclass Distance:
        Distance() except +
        Distance(int n, int d) except +
        void load_entry(double num, int row, int col)
        void compute_distances()
        double view_data(int i, int j)
        double lookup_distance(int i, int j)
        int size()