# -*- coding: utf-8 -*-
"""
Created on Tue May 07 11:08 2019

@author: agoldschmidt
"""

import numpy as np
import pychain as pyc
import pylinkage as pyl
try:
    from tqdm import tqdm_notebook, tqdm
except:
    None

# -------------------------------------------------------------------
# --- Utilites ----------------------------------------------------
# -------------------------------------------------------------------

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