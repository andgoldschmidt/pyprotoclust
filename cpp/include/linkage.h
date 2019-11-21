#ifndef LINKAGE_H
#define LINKAGE_H

#include "ltmatrix.h"
#include <memory>
#include <tuple>
#include <vector>

namespace minimax {
    class Linkage {
        public:
            Linkage() {};
            Linkage(std::shared_ptr<LTMatrix<float> > full_distance_matrix);
            
            /**
             * For each point in Gg+Hh, find the maximal radius at that point to
             * encircle the space. Then store the minimum such radius and the
             * corresponding center point in the member variables.
             * 
             * Returns:
             *      - A tuple containing the radius and center for the linkage
             * 
             * Requires:
             *      - this->distance_matrix must have entries for Gg and Hh
             **/
            std::tuple<double, int> minimax_linkage(const std::vector<int>& Gg, const std::vector<int>& Hh) const;

            /**
             *  Indirectly load G and H and indirectly return the radius and center.
             * 
             *  Requires:
             *      - this->G and this->H must be filled.
             * 
             *  Updates:
             *      - Stores the minimax this->distance and this->center point for G+H.
             */
            void minimax_linkage();

            /** Construct the list of indices G to merge, entry by entry **/
            void add_to_G(int entry);

            /** Construct the list of indices H to merge, entry by entry **/
            void add_to_H(int entry);

            /** Empty the indices from G and H to start fresh **/
            void clear_GH();

            // Setters and Getters
            double get_minimax_distance() { return this->distance; };
            int get_minimax_center() { return this->center; };

        private:
            std::shared_ptr<LTMatrix<float>> distance_matrix;
            std::vector<int> G;
            std::vector<int> H;

            double distance;
            int center;
            int n_elems;
    };
}

#endif