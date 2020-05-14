#ifndef PROTOCLUST_H
#define PROTOCLUST_H

#include "chain.h"
#include "linkage.h"
#include "ltmatrix.h"
#include <vector>
#include <memory>

namespace minimax {

    class Protoclust {

        public:
            Protoclust() {
                this->n_elems = 0;
            };
            Protoclust(int n);
            Protoclust(const std::vector< std::vector<float>>& dm);

            /**
             *  Set distance_matrix[i, j] = distance_matrix[j, i] = distance.
             **/
            void set_distance(int i, int j, float distance);

            /**
             * Computes the hierarchical clustering according to the minimax linkage.
             * 
             * Requires that the distance_matrix has been set to the desired values.
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
             *      (ii) The distance matrix is unchanged throughout.
             */
            void compute_index(const int i);

            // Accessors
            int get_Z_0(int i) { return this->Z_0[i]; };
            int get_Z_1(int i) { return this->Z_1[i]; };
            double get_Z_2(int i) { return this->Z_2[i]; };
            int get_Z_3(int i) { return this->Z_3[i]; };
            int get_cluster_center(int i) { return this->cluster_centers[i]; };

        private:
            int n_elems;

            // Full distance matrix (n_elems initial points and n_elems-1 joins)
            std::shared_ptr<LTMatrix<float> > full_distance_matrix;
            Chain chain;
            Linkage linkage;

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