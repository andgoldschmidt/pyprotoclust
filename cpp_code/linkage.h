#ifndef LINKAGE_H
#define LINKAGE_H

#include <vector>

namespace minimax {
    class Linkage {
        public:
            Linkage() {};
            Linkage(int size);
            
            /** Construct the list of indices G to merge, entry by entry **/
            void add_to_G(int entry);

            /** Construct the list of indices H to merge, entry by entry **/
            void add_to_H(int entry);

            /** Empty the indices from G and H to start fresh **/
            void clear_GH();
            
            /**
             * For each point in G+H, find the maximal radius at that point to
             * encircle the space. Then store the minimum such radius and the
             * corresponding center point in the member variables.
             * 
             * Requires:
             *   - this->G and this->H must be filled
             *   - this->distance_matrix must have entries for the indices 
             *     in this->G and this->H
             * 
             * Updates:
             *   - Stores the minimax this->distance and this->center point for G+H.
             **/
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