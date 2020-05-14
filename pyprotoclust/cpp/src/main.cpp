#include "ltmatrix.h"
#include "protoclust.h"
#include "chain.h"
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <iomanip>      // std::setw

int main(int argc, char ** argv) {
    int size = 3;
    std::vector< std::vector<float>> v = std::vector<std::vector<float>> (size, std::vector<float>(size, 0));

    // for (int i = 0; i < 10; ++i)
    //     std::rand();

    // auto m = minimax::LTMatrix(size);

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j <= i; ++j) {
            auto x = i == j? 0 : ((float) std::rand() / (RAND_MAX ));
            v[i][j] = x;
            v[j][i] = x;
            // m(i,j) = i*(i+1)/2 + j;
        }
    }

    minimax::Protoclust h = minimax::Protoclust(v);
    h.compute();
    return 0;
}
