# -*- coding: utf-8 -*-
"""
Created on Tue May 07 11:08 2019

@author: agoldschmidt
"""

import numpy as np
import pychain as pyc
import pylinkage as pyl
import itertools as it
try:
    from tqdm import tqdm_notebook, tqdm
except:
    None

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

def progress(iterable, verbose, notebook):
    if verbose:
        try:
            if notebook:
                return tqdm_notebook(iterable)
            else:
                return tqdm(iterable)
        except NameError:
            return iterable
    else:
        return iterable

# -------------------------------------------------------------------
# --- Main algorithm ------------------------------------------------
# -------------------------------------------------------------------


def protoclust(distance_matrix, verbose=False, notebook=False):
    '''
    Algorithm computes clusters according to the algorithm in Hierarchical Clustering 
        With Prototypes via Minimax Linkage by J. Bien and R. Tibshirani.
        
    Arguments:
        distance_matrix: numpy array of distances [i,j] = d(x_i, x_j). All access calls \
            are made as __getitem__((i,j)) i.e. all accessing within protoclust is made \
            using integer pairs.
        verbose: Set as true to show a status bar
        notebook: Set as true if using a jupyter notebook

    Returns:
        Z: hierarchical clustering default (see scipy docs)
        clustering: the set of indices of each index
        clustering_centers: the center of each index
        clustering_distances: the minimax radius measured from the center at each index
        
    '''
    n,_ = distance_matrix.shape

    chain = pyc.PyChain(n)
    linkage = pyl.PyLinkage(n)

    chain.initialize_distances(distance_matrix)
    linkage.initialize_distances(distance_matrix)

    # Stores the linkage matrix for scipy hierarchical methods
    Z = []

    # Start with C_0 = {{x_1},{x_2},...,{x_n}} and d({x_i},{x_j}) = d(x_i,x_j)
    clustering = [[i] for i in range(n)] # List of subsets of {0,1,...,n-1} (length = n + iterations)
    clustering_centers = [i for i in range(n)] # List of points in {0,1,...,n-1} (length = n + iterations)
    clustering_distances = [0 for i in range(n)] # List of positive real distances (length = n + iterations) (always increases)

    # Keep track of the available indices at each iteration (also have this hiding in chain)
    available_indices = [list(range(n))]

    # n-1 merges must occur (iteration denoted l in comments)
    for iteration in progress(range(0, n-1), verbose, notebook):
        chain.grow_rnn()
        RNN_pair = chain.get_rnn()
    
        # Label the clusters
        G1 = clustering[RNN_pair[0]]
        G2 = clustering[RNN_pair[1]]

        # Compute the minimax distances for the new G1, G2 using all underlying points
        G1G2_distance, G1G2_center = linkage.minimax_linkage(G1, G2)
        
        # Add the new cluster to the record
        clustering.append(G1 + G2)
        clustering_centers.append(G1G2_center)
        clustering_distances.append(G1G2_distance)

        # Update the new cluster distances for all available indices
        for a in available_indices[iteration]:
            if a not in RNN_pair:
                new_distance = linkage.minimax_linkage(G1+G2, clustering[a])[0]
                chain.update_distance(a, n + iteration, new_distance)

        # Update the linkage matrix  (see scipy.cluster.hierarchy.linkage)
        Z.append([*RNN_pair, G1G2_distance, len(G1+G2)])

        # i.  Update the available indices by removing the merged indices and adding a new index
        # ii. Remove the RNN pair from the end of the chain
        chain.remove_rnn(*RNN_pair, iteration)

        # Python index update
        available_indices.append([a for a in available_indices[iteration] if a not in RNN_pair] + [n + iteration])

    return Z, [clustering, clustering_centers, clustering_distances]