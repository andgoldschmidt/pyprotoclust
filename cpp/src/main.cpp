#include "protoclust.h"
#include "chain.h"
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <iomanip>      // std::setw

int main(int argc, char ** argv) {
    int size = 20;
    std::vector< std::vector<double>> v = std::vector<std::vector<double>> (size, std::vector<double>(size, 0));

    for (int i = 0; i < 10; ++i)
        std::rand();

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j <= i; ++j) {
            auto x = i == j? 0 : ((double) std::rand() / (RAND_MAX ));
            v[i][j] = x;
            v[j][i] = x;
        }
    }
    minimax::Protoclust h = minimax::Protoclust(v);
    h.compute();
    return 0;
}
