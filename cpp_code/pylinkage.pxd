cdef extern from "linkage.h":
    pass

cdef extern from "linkage.h" namespace "minimax":
    cdef cppclass Linkage:
        Linkage() except +
        Linkage(int) except +
        void minimax_linkage()
        float get_minimax_distance()
        int get_minimax_center()
        void set_distance(int x, int y, float distance)
        void add_to_G(int entry)
        void add_to_H(int entry)
        void clear_GH()