#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <list>
#include <random>
#include <limits>

namespace minimax {

    class Chain {
        public:

            // Default does no inits
            Chain () {};

            // Size is constrained by RAND_MAX and INT_MAX
            Chain(int size);

            /** 
             *  Iterate over available indices from the current chain to find the next pair of recurrent nearest neighbors
             **/
            void grow_chain();

            /**
             *  Update an entry of the full distance matrix
             **/
            void set_distance(int index1, int index2, float distance);

            /**
             *  Remove the last two elements of the chain
             **/
            void trim_chain();

            /**
             *  Using the current chain, update the available indicies for iteration
             **/
            void merge_indicies(int r1, int r2, int iteration);

            // Access the recurrent nearest neighbors after growing the chain
            bool can_grow() { return this->available_indicies.size() > 1; };
            int chain_end_1() { return this->chain.empty() ? -1 : this->chain.back(); };
            int chain_end_2() { return this->chain.size() < 2 ? -1 : this->chain[this->chain.size()-2]; };

        private:
            int n_elems;

            std::default_random_engine generator;

            std::vector<int> chain;
            std::vector<std::vector<float>> full_distance_matrix; // TODO: Make upper triangular (use float for memory)
            std::vector<int> available_indicies;

            /**
             *  Return the nearest neighbor of index from the set of possible_neighbors using distance.
             **/
            int nearest(const int index);
            
    };

}

#endif