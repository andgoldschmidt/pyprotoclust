#ifndef LTMATRIX_H
#define LTMATRIX_H

#include <vector>

namespace minimax {
    // Lower-triangular matrix class
    template <class T>
    class LTMatrix {
        public:
            LTMatrix() { this->s = 0; };
            LTMatrix(int n);

            T& operator()(int i, int j);

            T get(int i, int j) const;
            void set(int i, int j, T dij);

            // Return the size of (i,j < size)
            int size() { return this->s; };
        
        private:
            int s;

            /**
             * Lower triangular coordinates are given such that j<=i.
             * 
             * Every index k < i contributes the full size of n_k = {j: j <= k}.
             *  The total is \sum_{k=0}^{i-1} k+1 = i(i+1)/2.
             *  Then, add the index j to account for the column.
             * 
             * Example for i=4, j=2 (want spot 12 below):
             *  | 0  -  -  -  -  - ...|
             *  | 1  2  -  -  -  - ...|
             *  | 3  4  5  -  -  - ...|
             *  | 6  7  8  9  -  - ...|
             *  | 10 11 12 13 14 - ...|
             *  | ...   ^^         ...|
             * 
             * Lookup: 4(4+1)/2 + 2 = 12
             */
            std::vector<T> distance;
    };
}

#endif