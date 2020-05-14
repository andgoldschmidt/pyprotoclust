import c_protoclust
from tqdm import tqdm_notebook, tqdm


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


def protoclust(distance_matrix, verbose=False, notebook=False):
    n = len(distance_matrix)
    p = c_protoclust.PyProtoclust(n)
    p.initialize_distances(distance_matrix)
    for i in progress(range(n-1), verbose, notebook):
        p.compute_at(i)
    return p.Z(n), p.cluster_centers(n)

