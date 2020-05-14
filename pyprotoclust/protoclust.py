from pyprotoclust.c_protoclust import CyProtoclust
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
    """
    An implementatin of representative hierarchical clustering using minimax linkage.

    Args:
        distance_matrix (:obj:`ndarray` of float): A distance matrix to be clustered.
            Must be accessible with index pairs.
        verbose (bool): Optional. Print a progress bar. Default False.
        notebook (bool): Optional. Flag if using a jupyter notebook to allow progress bar to print. Default False.

    Returns:
        (tuple): tuple containing:

            - :obj:`ndarray`: Z
                The linkage matrix encoding the hierarchical clustering.
                See scipy.cluster.hierarchy.linkage for information about the format of Z.

            - :obj:`ndarray`: prototypes
                The prototypes associated with cluster at each linkage iteration.
                The length of this list is equal to the size of the input data plus the length of Z.

    """
    n = len(distance_matrix)
    p = CyProtoclust(n)
    p.initialize_distances(distance_matrix)
    for i in progress(range(n-1), verbose, notebook):
        p.compute_at(i)
    return p.Z(n), p.cluster_centers(n)

