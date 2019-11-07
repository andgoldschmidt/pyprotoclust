#include "protoclust.h"
#include "chain.h"
#include "linkage.h"
#include <algorithm>

namespace minimax{
    Protoclust::Protoclust(int n) {
        this->n_elems = n;

        // List of subsets of {0,1,...,n-1} (length = n + (n-1 merges))
        this->clustering.reserve(2*this->n_elems - 1);

        // List of points in {0,1,...,n-1} (length = n + (n-1 merges))
        this->clustering_centers.reserve(2*this->n_elems - 1);

        // Initialize indices
        for(int i = 0; i < this->n_elems; ++i){
            this->clustering[i] = std::vector<int>(1, i);
            this->clustering_centers[i] = i;
        }

        // Linkage matrix (length = n-1 merges)
        this->Z.reserve(this->n_elems - 1);
    }

    Protoclust::Protoclust(const std::vector< std::vector<double>>& dm)
                          : Protoclust::Protoclust(dm.size()) {
        this->distance_matrix = dm;

        // Need copies of the distance_matrix to initialize the chain and linkage
        this->linkage = Linkage(this->distance_matrix);
        this->chain = Chain(this->distance_matrix);
    }

    void Protoclust::compute() {
        // n.b. all members are initialized according to n_elems
        // n-1 merges must occur 
        for(int i=0; i < this->n_elems; ++i) {
            // allow this loop to occur outside this code (e.g. for status bars)
            this->compute_index(i);
        }
    }

    void Protoclust::compute_index(int i) {
            this->chain.grow_chain();
            int rnn1 = this->chain.chain_end_2();
            int rnn2 = this->chain.chain_end_1();

            // Label the clusters
            std::vector<int> G1 = this->clustering[rnn1];
            std::vector<int> G2 = this->clustering[rnn2];
            
            // Construct merged cluster
            // Resize (not reserve) initializes elements.
            int G1G2_size = G1.size() + G2.size();
            this->clustering[this->n_elems + i].resize(G1G2_size);
            std::merge(G1.begin(), G1.end(), G2.begin(), G2.end(), 
                       this->clustering[this->n_elems + i].begin());

            // Compute the minimax distances for the 
            //   new G1, G2 using all underlying points
            this->linkage.minimax_linkage(G1, G2);
            clustering_centers[this->n_elems + i] = this->linkage.get_minimax_center();
            double G1G2_distance = this->linkage.get_minimax_distance();

            // Update the linkage matrix
            this->Z[i] = std::make_tuple(rnn1, rnn2, G1G2_distance, G1G2_size);

            // Update cluster distances for (unmerged) available indices
            for(auto a : this->chain.get_available_indicies()) {
                if (a != rnn1 && a != rnn2) {
                    this->linkage.minimax_linkage(clustering[this->n_elems + i], clustering[a]);
                    this->chain.set_distance(a, this->n_elems+i, this->linkage.get_minimax_distance());
                }
            }

            // Update available indices by removing the merged indices 
            //  and adding the new index by its iteration count
            chain.merge_indicies(rnn1, rnn2, i);
            // Remove the RNN pair from the end of the chain
            chain.trim_chain();
    }
}