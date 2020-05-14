API Reference
=============

**Pyprotoclust** is an implementatin of representative hierarchical clustering using minimax linkage.

Pyprotoclust takes a distance matrix as input. It returns a linkage matrix encoding the hierachical clustering as well
as an additional list labelling the prototypes associated with each clustering. This allows a user to integrate with
the existing tools in the
`SciPy hierarchical clustering module <https://docs.scipy.org/doc/scipy/reference/cluster.hierarchy.html>`_.

.. autofunction:: pyprotoclust.protoclust

