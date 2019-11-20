#include "linkage.h"
#include <limits>


namespace minimax {
    Linkage::Linkage(int size) {
        this->n_elems = size;

        // Construct distance matrix with default 0 (no need to enter diagonal)
        this->distance_matrix = std::vector<std::vector<double>> (size, std::vector<double>(size, 0));

        // Initialize index sets
        this->G.reserve(size);
        this->H.reserve(size);
    }

    Linkage::Linkage(const std::vector<std::vector<double> >& dm)
                    : Linkage(dm.size()) {
        // Copy in this distance matrix
        this->distance_matrix = dm;
    }

    void Linkage::minimax_linkage() {
        std::tuple<double, int> result = this->minimax_linkage(this->G, this->H);
        this->clear_GH();
        this->distance = std::get<0>(result);
        this->center = std::get<1>(result);
    }

    std::tuple<double, int> const Linkage::minimax_linkage(const std::vector<int>& Gg, const std::vector<int>& Hh) {
        int best_center = -1;
        double best_radius = std::numeric_limits<double>::max();
        
        std::vector<int> G_union_H;
        G_union_H.reserve( Gg.size() + Hh.size() ); // preallocate memory
        G_union_H.insert( G_union_H.end(), Gg.begin(), Gg.end() );
        G_union_H.insert( G_union_H.end(), Hh.begin(), Hh.end() );

        // Get the minimal of the max radii
        for (int possible_center : G_union_H) {
            double current_max = -1;
            // Get the max radius
            for (int elem : G_union_H){
                auto r = this->distance_matrix[possible_center][elem];
                if (current_max < r)
                    current_max = r;
            }
            if (current_max < best_radius) {
                best_radius = current_max;
                best_center = possible_center;
            }
        }
        return std::make_tuple(best_radius, best_center);
    }

    void Linkage::set_distance(int i, int j, double dist) {
        this->distance_matrix[i][j] = dist;
        this->distance_matrix[j][i] = dist;
    }
    
    void Linkage::add_to_G(int entry) {
        this->G.emplace_back(entry);
    }
    void Linkage::add_to_H(int entry) {
        this->G.emplace_back(entry);
    }

    void Linkage::clear_GH() {
        this->G.clear();
        this->G.reserve(this->n_elems);
        this->H.clear();
        this->H.reserve(this->n_elems);
    }
}