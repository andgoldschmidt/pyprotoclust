#include "ltmatrix.h"

namespace minimax{
    // Explicit instantiations as needed
    template class LTMatrix<float>;

    template <class T>
    LTMatrix<T>::LTMatrix(int n){
        this->s = n;
        this->distance = std::vector<T>(n*(n+1)/2, 0);
    }

    template <class T>
    T& LTMatrix<T>::operator()(int i, int j)
    {
        if (j <= i) {
            return this->distance[i*(i+1)/2 + j];
        } else {
            return this->operator()(j,i);
        }
    }
    
    template <class T>
    void LTMatrix<T>::set(int i, int j, T dij) {
        if (j <= i) {
            this->distance[i*(i+1)/2 + j] = dij;
        } else {
            this->set(j, i, dij);
        }
    }

    template <class T>
    T LTMatrix<T>::get(int i, int j) const {
        if (j <= i) {
            return this->distance[i*(i+1)/2 + j];
        } else {
            return this->get(j, i);
        }
    }
}