#ifndef LINKAGE_H
#define LINKAGE_H

#include <vector>
#include <tuple>

namespace minimax {
    class Linkage {
        public:
            Linkage() {};
            Linkage(int size);
            Linkage(const std::vector<std::vector<double> >& dm);
            
            /** Construct the list of indices G to merge, entry by entry **/
            void add_to_G(int entry);

            /** Construct the list of indices H to merge, entry by entry **/
            void add_to_H(int entry);

            /** Empty the indices from G and H to start fresh **/
            void clear_GH();
            
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
            std::tuple<double, int> const minimax_linkage(const std::vector<int>& Gg, const std::vector<int>& Hh);

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

            // Setters and Getters
            double get_minimax_distance() { return this->distance; };
            int get_minimax_center() { return this->center; };
            void set_distance(int i, int j, double dist);

        private:
            std::vector<std::vector<double> > distance_matrix;
            std::vector<int> G;
            std::vector<int> H;

            double distance;
            int center;
            int n_elems;
    };
}

#endif