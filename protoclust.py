# -*- coding: utf-8 -*-
"""
Created on Tue May 07 11:08 2019

@author: agoldschmidt
"""

import numpy as np
import itertools as it

# -------------------------------------------------------------------
# --- Utilites ----------------------------------------------------
# -------------------------------------------------------------------

class distance_matrix:
    def __init__(self, X, distance_fn):
        '''
        Construct a distance matrix D={i,j, d(x_i,x_j): for all x_i, x_j in X}. 
            The matrix is symmetric so we just compute distances for all combinations 
            of len(X) choose 2. Use the lookup function 'lookup_distance' to get distances.
        
        Arguments:
            X: A list of elements from which to construct the distance matrix
            distance_fn: Takes a pair of rows of X and returns a real value

        Class members:
            distance_matrix: matrix of distances written [x1, x2, d(x1,x2)],\
                follows the structure set in construct_distance_matrix
            distance_fn: (stores argument)
            n_elems: number of points in the initial set creating distance_matrix
        '''
        self.distance_fn = distance_fn
        self.n_elems = len(X)
        self.shape = (self.n_elems, self.n_elems)
        self._distance_matrix = [[i,j,self.distance_fn(X[i], X[j])] for i,j in it.combinations(range(self.n_elems),2)]

    def __len__(self):
        return self.n_elems

    def __getitem__(self, key):
        try:
            # Always a tuple of ints, no slicing.
            k1, k2 = key
            return self.lookup_distance(k1, k2)
        except:
            # Failure to lookup.
            raise IndexError("Key must be a pair of integers.")

    def lookup_distance(self, i, j, equal_distance = 0):
        '''
        Every index k < i contributes the full size of the set n_k = {j: k<j & j < n}.
            The total is \sum_{k=0}^{i-1} (n-1)-i = i(n-1) - n(n-1)/2
            The difference j-i-1 is how far along in the set n_i to 
            go to find the desired distance.
            
        Arguments:
            i: index of distance matrix
            j: other index of distance matrix
            equal_distance: (Default 0) Distance of equivalent elements.
        '''
        if i==j:
            return equal_distance
        elif i < j:
            # Look up the correct row:
            return self._distance_matrix[i*(len(self)-1)-i*(i-1)//2 + j-i-1][2]
        else:
            return self.lookup_distance(j, i, equal_distance)

    def asarray(self):
        a = np.empty((len(self),len(self)))
        for i in range(len(self)):
            for j in range(len(self)):
                a[i,j] = self[i,j]
        return a


# -------------------------------------------------------------------

def find_nearest_neighbor(index, distance_matrix, possible_neighbors):
    '''
    Return the nearest neighbor of index from the set {0,1,2,...,n_elems-1}
        of possible_neighbors using distance.
    '''
    return min([[j, distance_matrix[index, j]] for j in possible_neighbors if j != index], key=lambda x: x[1])[0]


def minimax_distance(G, H, distance):
    '''
    For each point in G+H, find the maximal radius at that point to
        encircle the space. Then return the minimum such radius and the
        corresponding center point.  

    Arguments:
        distance: matrix storing the precomputed distances
    '''
    G_union_H = G + H # list of original indices
    best_center = -1
    best_center_r = np.inf
    for possible_center in G_union_H:
        center_max_r = max([distance[possible_center, j] for j in G_union_H])
        if center_max_r < best_center_r:
            best_center_r = center_max_r
            best_center = possible_center
    return [best_center_r, best_center]

# -------------------------------------------------------------------
# --- Main algorithm ------------------------------------------------
# -------------------------------------------------------------------


def protoclust(distance_matrix, verbose=False):
    '''
    Algorithm computes clusters according to the algorithm in Hierarchical Clustering 
        With Prototypes via Minimax Linkage by J. Bien and R. Tibshirani.
        
    Arguments:
        distance_matrix: array of distances [i,j] = d(x_i, x_j). All access calls \
            are made as __getitem__((i,j)) i.e. all accessing within protoclust is made \
            using integer pairs.

    Returns:
        
    '''
    n,_ = distance_matrix.shape

    # TODO: worry about memory optimization later (it's (2n-1)^2 vs 2n-1 choose 2)
    # Need a big matrix of size (2n-1)^2, prefilled n x n; Extra rows are added at each iteration
    big_matrix = np.inf*np.ones((2*n - 1, 2*n - 1))
    for i in range(n):
        for j in range(n):
            big_matrix[i,j] = distance_matrix[i,j]

    # Stores the linkage matrix for scipy
    Z = []

    # Start with C_0 = {{x_1},{x_2},...,{x_n}} and d({x_i},{x_j}) = d(x_i,x_j)
    clustering = [[i] for i in range(n)] # List of subsets of {0,1,...,n-1} (length = n + iterations)
    clustering_centers = [i for i in range(n)] # List of points in {0,1,...,n-1} (length = n + iterations)
    clustering_distances = [0 for i in range(n)] # List of positive real distances (length = n + iterations) (always increases)

    # The chain is empty
    chain = []

    # Keep track of the available indices at each iteration
    available_indices = [list(range(n))]

    # n-1 merges must occur (iteration denoted l in comments)
    for iteration in range(0, n-1):
        if verbose:
            print(iteration)
        # If chain is empty, choose an arbitrary index from those available at this iteration
        chain = chain if chain else [np.random.choice(available_indices[iteration])]

        # Grow a nearest neighbor chain until an repeat nearest neighbor pair (G1,G2) is found
        # This must succeeed within the available indices less 1 for the end of the chain (exception?)
        for igrowth in range(len(available_indices[iteration])-1):
            # Find the index of the closest entry
            neighbor = find_nearest_neighbor(chain[-1], big_matrix, available_indices[iteration])

            # Look for a loop in the nearest neighbor chain
            if len(chain) > 1 and chain[-2] == neighbor:
                break
            else:
                chain.append(neighbor)

        # Label the RNN pair
        G1 = clustering[chain[-1]]
        G2 = clustering[chain[-2]]
        RNN_pair = [chain[-2], chain[-1]]

        # Compute the minimax distances for the new G1 + G2 using all underlying points
        G1G2_distance, G1G2_center = minimax_distance(G1, G2, distance_matrix)
        
        # Add the new cluster to the record
        clustering.append(G1 + G2)
        clustering_centers.append(G1G2_center)
        clustering_distances.append(G1G2_distance)

        # Compute the new cluster distances for all available indices
        new_distance = [minimax_distance(G1+G2, clustering[a], distance_matrix)[0] if a not in RNN_pair else 0
                        for a in available_indices[iteration]]

        # Fill the n+iteration row/column of the big matrix at all available indices
        big_matrix[n + iteration, available_indices[iteration]] =  new_distance
        big_matrix[available_indices[iteration], n + iteration] =  new_distance

        # Update the linkage matrix  (see scipy.cluster.hierarchy.linkage)
        Z.append([*RNN_pair, G1G2_distance, len(G1+G2)])

        # Remove the RNN pair from the end of the chain
        chain = [c for c in chain if c not in RNN_pair]

        # Update the available indices by removing the merged indices and adding the new index
        available_indices.append([a for a in available_indices[iteration] if a not in RNN_pair] + [n + iteration])

    return Z, [clustering, clustering_centers, clustering_distances]