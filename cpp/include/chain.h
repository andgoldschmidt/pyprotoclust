#ifndef CHAIN_H
#define CHAIN_H

#include "ltmatrix.h"
#include <limits>
#include <memory>
#include <random>
#include <vector>

namespace minimax {

    class Chain {
        public:
            // Default does no inits
            Chain () {};

            // Size is constrained by RAND_MAX and INT_MAX
            Chain(std::shared_ptr<LTMatrix<float> > dm);

            /** 
             *  Iterate over available indices from the current chain to find the next pair of recurrent nearest neighbors
             **/
            void grow_chain();

            /**
             *  Remove the last two elements of the chain
             **/
            void trim_chain();

            /**
             *  Using the current chain, update the available indicies for iteration.
             * 
             *  TODO: Refactor iteration -> new index
             * 
             *  Parameters:
             *      int r1: remove from available indices
             *      int r2: remove from available indices
             *      int iteration:  new available index = iteration + number of original indices
             **/
            void merge_indicies(int r1, int r2, int iteration);

            // Access the recurrent nearest neighbors after growing the chain
            bool can_grow() { return this->available_indicies.size() > 1; };
            int chain_end_1() { return this->chain.empty() ? -1 : this->chain.back(); };
            int chain_end_2() { return this->chain.size() < 2 ? -1 : this->chain[this->chain.size()-2]; };

            // Read-only access to available indices
            const std::vector<int>& get_available_indicies() { return this->available_indicies; };

        private:
            int n_elems;

            std::default_random_engine generator;

            std::vector<int> chain;
            
            // Full distance matrix (n_elems initial points and n_elems-1 joins)
            std::shared_ptr<LTMatrix<float> > full_distance_matrix;

            std::vector<int> available_indicies;

            /**
             *  Return the nearest neighbor of index from the set of possible_neighbors using distance.
             **/
            int nearest(const int index);
            
    };

}

#endif