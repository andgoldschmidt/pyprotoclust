# -*- coding: utf-8 -*-
"""
Created on Tue May 07 11:08 2019

@author: agoldschmidt

TODO: I know that I won't always be able to store the distances in memory.
    I'll need to build in some adjustments for that. Should be that I refactor
    so that all functions that want distances call lookups. That's a major refactor.

The key idea of the implementation is that combinatorics outputs start with 0 and
    run over the entire range of other indices. The distance matrix will lose 2
    indices on joining, but when those are deleted that corresponds to a simple 
    reindexing of the space.

This does mean that all the other pieces in the game need reindexing, and that the
    clustering needs to match the new indices.
"""

import numpy as np
import itertools as it


# -------------------------------------------------------------------
# --- Utilites ------------------------------------------------------
# -------------------------------------------------------------------

def construct_distance_matrix(X, distance_fn):
    '''
    Construct a distance matrix D={i,j, d(x_i,x_j): for all x_i, x_j in X}. 
        The matrix is symmetric so we just compute distances for all combinations 
        of len(X) choose 2. Use the lookup function 'lookup_distance' to get distances.
        Currently, Euclidean distance is the fixed behavior.
    
    Arguments:
        X: A list of elements from which to construct the distance matrix
        distance_fn: Takes a pair of rows of X and returns a real value
    '''
    return [[i,j,distance_fn(X[i], X[j])] for i,j in it.combinations(range(len(X)),2)]

def lookup_distance(i, j, distance_matrix, n_elems, equal_distance = 0):
    '''
    Every index k < i contributes the full size of the set n_k = {j: k<j & j < n}.
        The total is \sum_{k=0}^{i-1} (n-1)-i = i(n-1) - n(n-1)/2
        The difference j-i-1 is how far along in the set n_i to 
        go to find the desired distance.
        
    Arguments:
        i: index of distance matrix
        j: other index of distance matrix
        distance_matrix: matrix of distances written [x1, x2, d(x1,x2)],\
                         follows the structure set in construct_distance_matrix
        n_elems: number of points in the initial set creating distance_matrix
        equal_distance: (Default 0) Distance of equivalent elements.
    '''
    if i==j:
        return equal_distance
    elif i < j:
        # Look up the correct row:
        return distance_matrix[i*(n_elems-1)-i*(i-1)//2 + j-i-1][2]
    else:
        return lookup_distance(j,i,distance_matrix,n_elems,equal_distance)


def find_nearest_neighbor(index, distance_matrix, n_elems):
    '''
    Return the nearest neighbor of index from the set {0,1,2,...,n_elems-1}
        of possible_neighbors using distance defined in distance_matrix.
    '''
    possible_neighbors = list(range(n_elems))
    return min([[j,lookup_distance(index, j, distance_matrix, n_elems)] 
                for j in possible_neighbors if j != index],
               key=lambda x: x[1])[0]

def minimax_distance(G, H, distance, n_elems):
    '''
    For each point in G+H, find the maximal radius at that point to
        encircle the space. Then return the minimum such radius and the
        corresponding center point.      
    '''
    G_union_H = G + H # list of original indices
    best_center = -1
    best_center_r = np.inf
    for center in G_union_H:
        center_max_r = max([lookup_distance(center, j, distance, n_elems)
                           for j in G_union_H if j != center])
        if center_max_r < best_center_r:
            best_center_r = center_max_r
            best_center = center
    return [best_center_r, best_center]

def update_i(index, rnn_pair):
    new_index = index
    # Correct indices for removed entries
    if index > rnn_pair[0]:
        new_index -= 1
    if index > rnn_pair[1]:
        new_index -= 1

    # Increment to make room for new entries
    new_index += 1
    return new_index

def update_distance(distance_matrix, rnn_pair):
    new_distance = []
    for d in distance_matrix:
        # Skip joined entries
        if d[0] in rnn_pair or d[1] in rnn_pair:
            continue
        else:
            new_distance.append([update_i(d[0], rnn_pair),
                                 update_i(d[1], rnn_pair),
                                 d[2]])
    return new_distance

def update_chain(chain, rnn_pair):
    new_chain = []
    for c in chain:
        # Skip joined entries
        if c in rnn_pair:
            continue
        else:
            new_chain.append(update_i(c, rnn_pair))
    return new_chain

# -------------------------------------------------------------------
# --- Main algorithm ------------------------------------------------
# -------------------------------------------------------------------


def protoclust(X, distance_fn):
    n,d = X.shape

    # Start with C_0 = {{x_1},{x_2},...,{x_n}} and d({x_i},{x_j})=d(x_i,x_j)
    clustering = [[[i] for i in range(n)]] # List of clustering for each iteration
    clustering_centers =  [[[i] for i in range(n)]]
    clustering_distances = [[0 for i in range(n)]]

    # TODO: Pre-allocation should be an option, not the rule.
    distance = construct_distance_matrix(X, distance_fn)
    original_distances = distance


    # The chain is empty (the chain will count indices for C_{l-1})
    chain = []
    iteration = 0

    # n-1 merges must occur (l counts merges)
    for l in range(1, n):
        # 4. If chain is empty, choose an arbitrary index for C_{l-1};
        #    otherwise, let G be the current end of the chain.
        chain = chain if chain else [np.random.choice(n-iteration)]

        # 5. Grow a nearest neighbor chain from G until an RNN pair (G1,G2) is found
        # There are n-1-iteration neighbors searchable from chain[-1] (safer than while loop i.m.o)
        for growth in range(n-iteration-1):
            # Find the index of the closest entry
            neighbor = find_nearest_neighbor(chain[-1], distance, n-iteration)

            if len(chain) > 1 and chain[-2] == neighbor:
                break
            else:
                chain.append(neighbor)

        # Update the clustering by pre-pending the new pair
        G1 = clustering[iteration][chain[-1]]
        G2 = clustering[iteration][chain[-2]]
        new_clustering = []
        new_clustering_centers = []
        new_clustering_distance = []
        for group, center, idistance in zip(clustering[iteration], 
                                           clustering_centers[iteration], 
                                           clustering_distances[iteration]):
            if group not in [G1, G2]:
                new_clustering.append(group)
                new_clustering_centers.append(center)
                new_clustering_distance.append(idistance)
        
        clustering.append([G1 + G2] + new_clustering)
        G1G2_distance, G1G2_center = minimax_distance(G1, G2, original_distances, n)
        clustering_centers.append([G1G2_center] + new_clustering_centers)
        clustering_distances.append([G1G2_distance] + new_clustering_distance)

        # Successfully created a new clustering, increment
        iteration += 1

        # Iterate over the new iteration clustering and compute the new cluster distances
        new_distance = [[0, i+1, minimax_distance(G1+G2, H, original_distances, n)[0]]
                          for i, H in enumerate(clustering[iteration][1:])]
        distance = new_distance + update_distance(distance, [chain[-1], chain[-2]])
        
        # Correct the indices in the chain
        chain = update_chain(chain, [chain[-1], chain[-2]])

    return clustering, clustering_centers, clustering_distances