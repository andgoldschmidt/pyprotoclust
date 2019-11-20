#ifndef PROTOCLUST_H
#define PROTOCLUST_H

#include "chain.h"
#include "linkage.h"
#include <vector>

namespace minimax {

    class Protoclust {

        public:
            Protoclust() {
                this->n_elems = 0;
            };
            Protoclust(int n);
            Protoclust(const std::vector< std::vector<double>>& dm);

            /**
             *  Set distance_matrix[i, j] = distance_matrix[j, i] = distance.
             **/
            void set_distance(int i, int j, double distance);

            /**
             * Computes the hierarchical clustering according to the minimax linkage.
             * 
             * Requires that the distance_matrix has been set to the desired values. Assume 
             * that the internal state is such that the chain and linkage objects are now
             * built with the current distance_matrix.
             */
            void compute();

            /**
             * This function computes an iteration of the linkage algorithm (there are n_elems-1 
             * total such linkages). This function is exposed to allow external programs to manage 
             * the iteration process to better report the progress of the algorithm. In most cases,
             * a user should call compute to run the entire hierarchical clustering algorithm.
             * 
             * Assumes:
             *      (i) Argument i < n_elems - 1 (number of allowed merges). The internal
             *          state of this class is assumed to be such that {0,1,...,i-1} were
             *          called previous to the current argument i.
             *      (ii) The distance matrix is unchanged (TODO: Throw exception?)
             */
            void compute_index(const int i);

            // Get Z
            int get_Z_0(int i) { return this->Z_0[i]; };
            int get_Z_1(int i) { return this->Z_1[i]; };
            double get_Z_2(int i) { return this->Z_2[i]; };
            int get_Z_3(int i) { return this->Z_3[i]; };

            // Get clusters and cluster centers
            std::vector<int> get_cluster(int i) { return this->cluster[i]; }; // TODO: How to handle?
            int get_cluster_center(int i) { return this->cluster_centers[i]; };


        private:
            int n_elems;
            std::vector< std::vector<double>> distance_matrix; // n_elems by n_elems
            Chain chain;
            Linkage linkage;

            /**
             *  Pass completely assembled distance matrix to the internal chain and linkage.
             */ 
            void finalize_distance();

            // Used for finalize_distance() when incrementally setting distance matrix
            bool needs_finalization;

            /** 
             * Elements of the n-1 by 4 linkage matrix (for scipy.cluster.hierarchy.linkage)
             *   Z[i, 0] and Z[i, 1] are combined to form cluster n+i. 
             *   The distance between clusters Z[i, 0] and Z[i, 1] is given by Z[i, 2]
             *   Z[i, 3] represents the number of original observations in the newly formed cluster
             */
            std::vector<int> Z_0;
            std::vector<int> Z_1;
            std::vector<double> Z_2;
            std::vector<int> Z_3;
            void update_Z(int i, int i0, int i1, double i2, int i3);

            // The original indices comprising the cluster associated with each index
            std::vector< std::vector<int>> cluster; // Length: 2 n_elems -1

            // The original index associated with the center of each index.
            std::vector<int> cluster_centers; // Length: 2 n_elems -1

    };

}

#endif