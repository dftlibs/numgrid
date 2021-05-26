.. image:: https://github.com/dftlibs/numgrid/workflows/Test/badge.svg
   :target: https://github.com/dftlibs/numgrid/actions
.. image:: https://img.shields.io/badge/license-%20MPL--v2.0-blue.svg
   :target: LICENSE
.. image:: https://badge.fury.io/py/numgrid.svg
   :target: https://badge.fury.io/py/numgrid
.. image:: https://zenodo.org/badge/DOI/10.5281/zenodo.1470276.svg
   :target: https://doi.org/10.5281/zenodo.1470276

- `Changelog <CHANGES.rst>`__
-  Licensed under `MPL v2.0 <LICENSE>`__ (except John
   Burkardt’s Lebedev code which is redistributed under LGPL v3.0)


Numgrid
=======

Numgrid is a library that produces numerical integration grid for
molecules based on atom coordinates, atom types, and basis set
information. This library provides Rust and Python bindings.


.. contents:: Table of Contents


Who are the people behind this code?
====================================

Authors
-------

-  Radovan Bast


Contributors
------------

-  Roberto Di Remigio (OS X testing, streamlined Travis testing, better
   C++, error handling)

For a list of all the contributions see
https://github.com/dftlibs/numgrid/contributors.


Acknowledgements
----------------

-  Simon Neville (reporting issues)
-  Jaime Axel Rosal Sandberg (reporting issues)

This tool uses SPHERE_LEBEDEV_RULE, a C library written by John Burkardt which
computes a Lebedev quadrature rule over the surface of the unit sphere in 3D,
see also:
http://people.sc.fsu.edu/~jburkardt/c_src/sphere_lebedev_rule/sphere_lebedev_rule.html

This library uses and acknowledges the
MolSSI BSE (https://molssi-bse.github.io/basis_set_exchange/),
which is a rewrite of the Basis Set Exchange
(https://bse.pnl.gov/bse/portal) and is a collaboration between the Molecular
Sciences Software Institute (http://www.molssi.org) and the Environmental
Molecular Sciences Laboratory (https://www.emsl.pnl.gov).


Citation
--------

If you use this tool in a program or publication, please acknowledge its
author(s)::

  @misc{numgrid,
    author    = {Bast, Radovan},
    title     = {Numgrid: Numerical integration grid for molecules},
    month     = {1},
    year      = {2021},
    publisher = {Zenodo},
    version   = {v2.0.2},
    doi       = {10.5281/zenodo.1470276},
    url       = {https://doi.org/10.5281/zenodo.1470276}
  }

  @misc{sphere_lebedev_rule,
    author = {Burkardt, John},
    title  = {SPHERE_LEBEDEV_RULE: Quadrature Rules for the Unit Sphere},
    year   = {2010},
    url    = {https://people.sc.fsu.edu/~jburkardt/c_src/sphere_lebedev_rule/sphere_lebedev_rule.html}
  }

I kindly ask you to also cite the latter since Numgrid is basically a "shell"
around SPHERE_LEBEDEV_RULE, with added radial integration and molecular
partitioning.


Would you like to contribute?
-----------------------------

Yes please! Please follow `this excellent
guide <http://www.contribution-guide.org>`__. We do not require any
formal copyright assignment or contributor license agreement. Any
contributions intentionally sent upstream are presumed to be offered
under terms of the Mozilla Public License Version 2.0.


Requirements
============

-  Python (3.6 - 3.10).
-  For the Rust version: A `Rust installation <https://www.rust-lang.org/tools/install>`__.


Installation
============

Installing via pip
------------------

::

   python -m pip install numgrid


Building from sources and testing
---------------------------------

Building the code::

   cargo build --release

Testing the Rust interface::

   cargo test --release

Running also the longer tests::

   cargo test --release -- --ignored

Testing the Python layer::

   pip install -r requirements.txt  # ideally into a virtual environment
   maturin develop
   pytest tests/test.py


API
===

The API changed
---------------

The API changed (sorry!) for easier maintenance and simpler use:

- No initialization or deallocation necessary.

- One-step instead of two steps (since the radial grid generation time is
  negligible compared to space partitioning, it did not make sense anymore to
  separate these steps and introduce a state).

- ``alpha_min`` is given as dictionary which saves an argument and simplifies
  explaining the API.

- The library now provides Rust and Python bindings. It used to provide C and
  Fortran bindings. The C/Fortran code lives on the `cpp-version branch
  <https://github.com/dftlibs/numgrid/tree/cpp-version>`__.  I might bring the
  C interfaces back into the Rust code if there is sufficient interest/need.

- Note that the API will probably change again as soon as support for more
  quadratures is added (see `issue 43
  <https://github.com/dftlibs/numgrid/issues/43>`__).


Units
-----

Coordinates are in bohr.


Python example
--------------

As an example let us generate a grid for the water molecule:

.. code:: python

   import numgrid

   radial_precision = 1.0e-12
   min_num_angular_points = 86
   max_num_angular_points = 302

   proton_charges = [8, 1, 1]

   center_coordinates_bohr = [(0.0, 0.0, 0.0), (1.43, 0.0, 1.1), (-1.43, 0.0, 1.1)]

   # cc-pVDZ basis
   alpha_max = [
       11720.0,  # O
       13.01,  # H
       13.01,  # H
   ]
   alpha_min = [
       {0: 0.3023, 1: 0.2753, 2: 1.185},  # O
       {0: 0.122, 1: 0.727},  # H
       {0: 0.122, 1: 0.727},  # H
   ]

   for center_index in range(len(center_coordinates_bohr)):
       # atom grid using explicit basis set parameters
       coordinates, weights = numgrid.atom_grid(
           alpha_min[center_index],
           alpha_max[center_index],
           radial_precision,
           min_num_angular_points,
           max_num_angular_points,
           proton_charges,
           center_index,
           center_coordinates_bohr,
           hardness=3,
       )

       # atom grid using basis set name
       # this takes a second or two for the REST API request
       coordinates, weights = numgrid.atom_grid_bse(
           "cc-pVDZ",
           radial_precision,
           min_num_angular_points,
           max_num_angular_points,
           proton_charges,
           center_index,
           center_coordinates_bohr,
           hardness=3,
       )

   # radial grid (LMG) using explicit basis set parameters
   radii, weights = numgrid.radial_grid_lmg(
       alpha_min={0: 0.3023, 1: 0.2753, 2: 1.185},
       alpha_max=11720.0,
       radial_precision=1.0e-12,
       proton_charge=8,
   )

   # radial grid (LMG) using basis set name
   radii, weights = numgrid.radial_grid_lmg_bse(
       basis_set="cc-pVDZ",
       radial_precision=1.0e-12,
       proton_charge=8,
   )

   # radial grid with 100 points using Krack-Koster approach
   radii, weights = numgrid.radial_grid_kk(num_points=100)

   # angular grid with 14 points
   coordinates, weights = numgrid.angular_grid(num_points=14)


Notes and recommendations
-------------------------

- The smaller the ``radial_precision``, the better grid.

- For ``min_num_angular_points`` and ``max_num_angular_points``, see “Angular
  grid” below.

- ``alpha_max`` is the steepest basis set exponent.

- ``alpha_min`` is a dictionary and holds the smallest exponents for each
  angular momentum (order does not matter).

- Using ``center_index`` we tell the code which of the atom centers is the one
  we have computed the grid for.

- ``num_angular_grid_points`` has to be one of the many supported Lebedev grids
  (see table on the bottom of this page).


Rust interface
--------------

Needs to be documented better but the library exposes functions with the same
name as the Python interface and probably the best example on how it can be
used are the `integration tests
<https://github.com/dftlibs/numgrid/blob/main/tests/integration_test.rs>`__.


Saving grid in NumPy format
---------------------------

The current API makes is relatively easy to export the computed grid in NumPy format.

In this example we save the angular grid coordinates and weights to two separate files
in NumPy format:

.. code:: python

   import numgrid
   import numpy as np

   coordinates, weights = numgrid.angular_grid(14)

   np.save("angular_grid_coordinates.npy", coordinates)
   np.save("angular_grid_weights.npy", weights)


Parallelization
===============

The Becke partitioning step is parallelized using `Rayon
<https://github.com/rayon-rs/rayon>`__.  In other words, this step should be
able to use all available cores on the computer or computing node.  Since grids
are currently generated atom by atom, it is also possible to parallelize
"outside" by the caller.


Space partitioning
==================

The molecular integration grid is generated from atom-centered grids by
scaling the grid weights according to the Becke partitioning scheme,
`JCP 88, 2547 (1988) <http://dx.doi.org/10.1063/1.454033>`__. The
default Becke hardness is 3.


Radial grid
===========

Two choices are available:
- Lindh-Malmqvist-Gagliardi (https://dx.doi.org/10.1007/s002140100263)
- Krack-Köster (https://doi.org/10.1063/1.475719)

Advantage of LMG scheme: The range of the radial grid is basis set dependent.
The precision can be tuned with one single radial precision parameter. The
smaller the radial precision, the better quality grid you obtain.  The basis
set (more precisely the Gaussian primitives/exponents) are used to generate the
atomic radial grid range. This means that a more diffuse basis set generates a
more diffuse radial grid.

Advantage of the KK scheme: parameter-free.


Angular grid
============

The angular grid is generated according to Lebedev and Laikov [A
quadrature formula for the sphere of the 131st algebraic order of
accuracy, Russian Academy of Sciences Doklady Mathematics, Volume 59,
Number 3, 1999, pages 477-481].

The angular grid is pruned. The pruning is a primitive linear
interpolation between the minimum number and the maximum number of
angular points per radial shell. The maximum number is reached at 0.2
times the Bragg radius of the center.

The higher the values for minimum and maximum number of angular points,
the better.

For the minimum and maximum number of angular points the code will use
the following table and select the closest number with at least the
desired precision::

   {6,    14,   26,   38,   50,   74,   86,   110,  146,
    170,  194,  230,  266,  302,  350,  434,  590,  770,
    974,  1202, 1454, 1730, 2030, 2354, 2702, 3074, 3470,
    3890, 4334, 4802, 5294, 5810}

Taking the same number for the minimum and maximum number of angular
points switches off pruning.
