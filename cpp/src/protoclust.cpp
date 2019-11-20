#include "protoclust.h"
#include <algorithm>

namespace minimax{
    Protoclust::Protoclust(int n) {
        this->n_elems = n;

        // Construct matrix with zeros (don't initialize change and linkage yet)
        this->distance_matrix = std::vector<std::vector<double>> (this->n_elems, std::vector<double>(this->n_elems, 0));
        this->needs_finalization = true;

        // List of subsets of {0,1,...,n-1} (length = n + (n-1 merges))
        this->cluster.resize(2*this->n_elems - 1);

        // List of points in {0,1,...,n-1} (length = n + (n-1 merges))
        this->cluster_centers.resize(2*this->n_elems - 1);

        // Initialize indices
        for(int i = 0; i < this->n_elems; ++i){
            this->cluster[i] = std::vector<int>(1, i);
            this->cluster_centers[i] = i;
        }

        // Linkage matrix (length = n-1 merges)
        this->Z_0.resize(this->n_elems - 1);
        this->Z_1.resize(this->n_elems - 1);
        this->Z_2.resize(this->n_elems - 1);
        this->Z_3.resize(this->n_elems - 1);
    }

    Protoclust::Protoclust(const std::vector< std::vector<double>>& dm)
                          : Protoclust(dm.size()) {
        this->distance_matrix = dm;

        // Pass copies of the distance_matrix to initialize the chain and linkage
        this->linkage = Linkage(this->distance_matrix);
        this->chain = Chain(this->distance_matrix);
        this->needs_finalization = false;
    }

    void Protoclust::set_distance(int i, int j, double dist) {
        // Distance was updated, reset needs_finalization
        this->needs_finalization = true;
        this->distance_matrix[i][j] = dist;
        this->distance_matrix[j][i] = dist;
    }

    // TODO: If keeping set_distance, need toggle for this function to 
    // automatically apply at compute when needed.
    void Protoclust::finalize_distance() {
        this->linkage = Linkage(this->distance_matrix);
        this->chain = Chain(this->distance_matrix);
        this->needs_finalization = false;
    }

    void Protoclust::compute() {
        // n.b. all members are initialized according to n_elems
        // n-1 merges must occur 
        for(int i=0; i < this->n_elems - 1; ++i) {
            // allow this loop to occur outside this code (e.g. for status bars)
            this->compute_index(i);
        }
    }

    void Protoclust::compute_index(const int i) {
            if(i==0 && this->needs_finalization)
                this->finalize_distance(); // TODO: Exception if i!=0 and finalization needed?

            this->chain.grow_chain();
            int rnn1 = this->chain.chain_end_2();
            int rnn2 = this->chain.chain_end_1();

            // Label the clusters
            std::vector<int> G1 = this->cluster[rnn1];
            std::vector<int> G2 = this->cluster[rnn2];
            
            // Construct merged cluster
            // Resize (not reserve) initializes elements.
            int G1G2_size = G1.size() + G2.size();
            this->cluster[this->n_elems + i].resize(G1G2_size);
            std::merge(G1.begin(), G1.end(), G2.begin(), G2.end(), 
                       this->cluster[this->n_elems + i].begin());

            // Compute the minimax distances for the 
            //   new G1, G2 using all underlying points
            std::tuple<double, int> G1_G2_res = this->linkage.minimax_linkage(G1, G2);
            double G1G2_distance = std::get<0>(G1_G2_res);
            cluster_centers[this->n_elems + i] = std::get<1>(G1_G2_res);

            // Update the linkage matrix
            this->update_Z(i, rnn1, rnn2, G1G2_distance, G1G2_size);

            // Update cluster distances for (unmerged) available indices
            #pragma omp parallel for
            for(unsigned int ia=0; ia <  this->chain.get_available_indicies().size(); ++ia) {
                int a = this->chain.get_available_indicies()[ia];
                if (a != rnn1 && a != rnn2) {
                    std::tuple<double, int> result = this->linkage.minimax_linkage(cluster[this->n_elems + i], cluster[a]);
                    double distance = std::get<0>(result);
                    this->chain.set_distance(a, this->n_elems+i, distance);
                }
            }

            // Update available indices by removing the merged indices 
            //  and adding the new index by its iteration count
            chain.merge_indicies(rnn1, rnn2, i);
            // Remove the RNN pair from the end of the chain
            chain.trim_chain();
    }

    void Protoclust::update_Z(int i, int i0, int i1, double i2, int i3) {
        this->Z_0[i] = i0;
        this->Z_1[i] = i1;
        this->Z_2[i] = i2;
        this->Z_3[i] = i3;
    }

}