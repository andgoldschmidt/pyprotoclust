#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H

#include <vector>
#include <random>
#include <limits>


namespace minimax {
    /**
     *  The strategy with distance matrix is to provide a few default metrics
     *  built in to make the distance calculations fast.
     **/
    class Distance {
        public:
            Distance() {};
            
            /**
             * Input data is n elements by d features.
             **/
            Distance(int n, int d);

            void load_entry(double entry, int row, int col);

            double view_data(int i, int j);

            /**
             * Requries that all input data has been loaded into
             * the (n x d) matrix this->data.
             * 
             * Computes this->data_matrix.
             **/
            void compute_distances();

            /**
             * Look up d(i,j) in this->distance_matrix.
             **/
            double lookup_distance(int i, int j);

            int size() { return this->data.size(); };

        private:
            int n_elems; // length n
            int dim_data; // length d
            std::vector<double> distance_entry;
            std::vector<double> distance_matrix; // d(i,j) counted i < j (n choose 2 x 1)
            std::vector<std::vector<double> > data; // (n x d)

            void OutOfRangeException(int row, int col, std::string fname);

            /**
             * Compute d(i,j) using this->data[i] and this->data[j].
             **/
            double get_pairwise_distance(int i, int j);

            template<class T1, class T2>
            double eucl_distance(T1 first, T1 last, T2 first2);
    };
}

#endif