#include "chain.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace minimax{

    // -- Public

    Chain::Chain (int size) {
        // Requires 2*SIZE - 1 less than RAND_MAX and INT_MAX

        // Construct random number generator
        std::random_device rand_dev;
        this->generator = std::default_random_engine(rand_dev());

        // Construct full matrix (n initial and n-1 joins) with default 0 (no need to enter diagonal)
        this->full_distance_matrix = std::vector<std::vector<float>> (2*size-1, std::vector<float>(2*size-1, 0));

        // Max chain size
        this->chain.reserve(size);

        // Available indices are originally {0,1,...,n-1}
        this->available_indicies.reserve(size);
        for (int i = 0; i < size; ++i)
            this->available_indicies.emplace_back(i);

        this->n_elems = size;
    }

    void Chain::grow_chain() {
        // TODO: How should chain handle equal entries in neighbor search? (causes self-loops)

        // Empty? Randomly start chain 
        if (this->chain.empty()) {
            std::uniform_int_distribution<int>  distr(0, this->available_indicies.size()-1);
            int r = distr(this->generator);
            this->chain.emplace_back(this->available_indicies[r]);
        }

        // Guaranteed to exit before completing this worst-case loop
        for (int i = 0; i < this->available_indicies.size() - 1; ++i) {
            int neighbor = this->nearest(this->chain.back());
            // Check for a recurrent nearest neighbor (in chain: {..ab}, neighbor: a)
            if (this->chain.size() > 1 && this->chain[this->chain.size()-2] == neighbor)
                break;
            else
                this->chain.emplace_back(neighbor);
        }
    }

    void Chain::set_distance(int index1, int index2, float distance) {
        this->full_distance_matrix[index1][index2] = distance;
        this->full_distance_matrix[index2][index1] = distance;
    }

    void Chain::merge_indicies(int r1, int r2, int iteration) {
        // TODO: Refactor? 2*n instead of n
        this->available_indicies.erase(
             std::remove( this->available_indicies.begin(), this->available_indicies.end(), r1 ),
             this->available_indicies.end() );
        this->available_indicies.erase(
             std::remove( this->available_indicies.begin(), this->available_indicies.end(), r2 ),
             this->available_indicies.end() );

        // The new index reps the joining of the removed pair
        this->available_indicies.emplace_back(this->n_elems + iteration); 
    }

    // NOTE: 
    // Fails to remove all occurences of end values in the case of self loops caused
    // by equal values.
    void Chain::trim_chain() {
        int remove_two = 0;
        while (!this->chain.empty() && remove_two < 2) {
            this->chain.pop_back();
            remove_two++;
        }
    }

    // -- Private

    int Chain::nearest(int index) {
        // Default
        int nearest = -1;
        float nearest_dist = std::numeric_limits<float>::max();
        
        for (auto j : this->available_indicies) {
            if (j == index)
                continue;
            else if (this->full_distance_matrix[index][j] < nearest_dist) {
                nearest = j;
                nearest_dist = this->full_distance_matrix[index][j];
            }
        }

        if (nearest == -1) {
            std::stringstream s;
            s << "In Chain::nearest, no nearest neighbor found for " << std::to_string(index);
            throw std::runtime_error(s.str());
        }

        return nearest;
    }

};