# -*- coding: utf-8 -*-
"""
Created on Wed June 05 09:17 2019

@author: agoldschmidt
"""

import numpy as np
import itertools as it

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