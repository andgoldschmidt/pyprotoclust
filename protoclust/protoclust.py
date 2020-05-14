# -*- coding: utf-8 -*-
"""
Created on Tue May 07 11:08 2019

@author: agoldschmidt
"""

import pyprotoclust as pyp
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
    n = len(distance_matrix)
    p = pyp.PyProtoclust(n)
    p.initialize_distances(distance_matrix)
    for i in progress(range(n-1), verbose, notebook):
        p.compute_at(i)
    return p.Z(n), p.cluster_centers(n)