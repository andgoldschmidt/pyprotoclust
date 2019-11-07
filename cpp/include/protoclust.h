#ifndef PROTOCLUST_H
#define PROTOCLUST_H

#include "chain.h"
#include "linkage.h"
#include <vector>
#include <tuple>

namespace minimax {

    class Protoclust {

        public:
            Protoclust() {
                this->n_elems = 0;
            };
            Protoclust::Protoclust(int n);
            Protoclust::Protoclust(const std::vector< std::vector<double>>& dm);

            /**
             * 
             * Requires:
             *      distance_matrix
             *      chain
             *      linkage
             */
            void compute();
            void compute_index(int i);

            /** 
             * Elements of the n-1 by 4 linkage matrix (for scipy.cluster.hierarchy.linkage)
             *   Z[i, 0] and Z[i, 1] are combined to form cluster n+i. 
             *   The distance between clusters Z[i, 0] and Z[i, 1] is given by Z[i, 2]
             *   Z[i, 3] represents the number of original observations in the newly formed cluster
             */
            std::vector< std::tuple<int, int, double, int>> Z;

        private:
            int n_elems;
            std::vector< std::vector<double>> distance_matrix; // n_elems by n_elems
            Chain chain;
            Linkage linkage;

            // The original indices comprising the cluster associated with each index
            std::vector< std::vector<int>> clustering; // Length: 2 n_elems -1

            // The original index associated with the cneter of each index.
            std::vector<int> clustering_centers; // Length: 2 n_elems -1


    };

}

#endif