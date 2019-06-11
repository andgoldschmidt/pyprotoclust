#include "distance.h"
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace minimax{
    Distance::Distance(int n, int d) {
        this->n_elems = n;
        this->dim_data = d;

        // preallocate distance_matrix with n choose 2 entries
        size_t nchoose2 = (n*(n-1))/2;
        this->distance_matrix.reserve(nchoose2);

        // preallocate and initialize data with zeros... TODO: Better plan?
        this->data = std::vector<std::vector<double>> (n, std::vector<double>(d, 0));
    }

    void Distance::OutOfRangeException(int row, int col, std::string fname) {
        if (row > this->n_elems) {
            std::stringstream s;
            s << "In Distance::" << fname  << ", row exceeds data size " << std::to_string(this->n_elems);
            throw std::runtime_error(s.str());
        } else if (col > this->dim_data) {
            std::stringstream s;
            s << "In Distance::" << fname  << ", column exceeds feature length " << std::to_string(this->dim_data);
            throw std::runtime_error(s.str());
        } else {
            std::stringstream s;
            s << "In Distance::" << fname  << ", caught Exception::out_of_range.";
            throw std::runtime_error(s.str());
        }
    }

    void Distance::load_entry(double entry, int row, int col) {
        try {
            this->data[row][col] = entry;
        }
        catch (const std::out_of_range& oor) {
            this->OutOfRangeException(row, col, "load_entry");
        }
    }

    double Distance::view_data(int row, int col) {
        try {
            return this->data[row][col];
        }
        catch (const std::out_of_range& oor) {
            this->OutOfRangeException(row, col, "view_data");
        }
    }

    double Distance::lookup_distance(int i, int j) {
        // Every index k < i contributes the full size of the set n_k = {j: k < j & j < n}.
        // The total is \sum_{k=0}^{i-1} (n-1)-i = i(n-1) - n(n-1)/2
        // The difference j-i-1 is how far along in the set n_i until the desired distance.
        if (i==j)
            return 0;
        else if (i < j) {
            // Look up the correct row:
            int row = i*(this->n_elems-1)-i*(i-1)/2 + j-i-1;
            return this->distance_matrix[row];
        }
        else
            return this->lookup_distance(j, i);
    }

    void Distance::compute_distances() {
        if (this->data.size() < 1) {
            std::stringstream s;
            s << "In Distance::compute_distances, cannot compute distances for data size " << std::to_string(this->data.size());
            throw std::runtime_error(s.str());
        }

        // Must abide by the rules set in lookup_distance re: order
        // append entry (i,j) with i < j, in order of increasing j
        // We only store combinations (i /= j)
        for (int i = 0; i < this->data.size(); ++i)
            for (int j = i+1; j < this->data.size(); ++j)
                this->distance_matrix.emplace_back(this->get_pairwise_distance(i,j));
    }

    double Distance::get_pairwise_distance(int i, int j) {
        return this->eucl_distance(this->data[i].begin(), this->data[i].end(), this->data[j].begin());
    }

    // TODO: Just some rando implementation, do better?
    template<class T1, class T2>
    double Distance::eucl_distance(T1 first, T1 last, T2 first2) {
        double ret = 0.0;
        while (first != last) {
            double dist = (*first++) - (*first2++);
            ret += dist * dist;
        }
        return ret > 0.0 ? std::sqrt(ret) : 0.0;
    }
}
