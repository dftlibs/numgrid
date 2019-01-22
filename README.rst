.. image:: https://travis-ci.org/dftlibs/numgrid.svg?branch=master
   :target: https://travis-ci.org/dftlibs/numgrid/builds
.. image:: https://coveralls.io/repos/dftlibs/numgrid/badge.png?branch=master
   :target: https://coveralls.io/r/dftlibs/numgrid?branch=master
.. image:: https://img.shields.io/badge/license-%20MPL--v2.0-blue.svg
   :target: LICENSE
.. image:: https://badge.fury.io/py/numgrid.svg
   :target: https://badge.fury.io/py/numgrid
.. image:: https://zenodo.org/badge/DOI/10.5281/zenodo.1470277.svg
   :target: https://doi.org/10.5281/zenodo.1470277

- `Changelog <CHANGES.rst>`__
-  `Build and test
   history <https://travis-ci.org/dftlibs/numgrid/builds>`__
-  `Code coverage <https://coveralls.io/r/dftlibs/numgrid>`__
-  Licensed under `MPL v2.0 <LICENSE>`__ (except John
   Burkardt’s Lebedev code which is redistributed under LGPL v3.0)
-  Built with `Autocmake <https://github.com/coderefinery/autocmake>`__


About
=====

numgrid is a library that produces numerical integration grid for
molecules based on atom coordinates, atom types, and basis set
information. This library can be built with C, Fortran, and Python bindings.


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
author(s). The metadata necessary for citing this tool can be found in the
`CITATION.cff <CITATION.cff>`__ file. For more information CITATION.cff files, see
https://citation-file-format.github.io.


Would you like to contribute?
-----------------------------

Yes please! Please follow `this excellent
guide <http://www.contribution-guide.org>`__. We do not require any
formal copyright assignment or contributor license agreement. Any
contributions intentionally sent upstream are presumed to be offered
under terms of the Mozilla Public License Version 2.0.


Requirements
============

-  CMake
-  C and C++ compiler
-  Fortran compiler (to build the optional Fortran interface)
-  `CFFI <https://cffi.readthedocs.org>`__ (to access the optional
   Python interface)
-  `pytest <http://pytest.org>`__ (to test the optional Python
   interface)


Installation
============

Installing via pip
------------------

::

   pip install numgrid


Building and testing from sources
---------------------------------

Fetch the code::

   git clone https://github.com/dftlibs/numgrid.git

Install Python dependencies (optional)::

   pipenv install

Build the code::

   cd numgrid
   ./setup --fc=gfortran --cc=gcc --cxx=g++
   cd build
   make
   make test

The Python interface is automatically tested by Travis CI:
https://github.com/dftlibs/numgrid/blob/master/.travis.yml


API
===

The library provides a context-aware C interface. In addition it also
provides a Fortran and Python interfaces as thin layers on top of the C
interface::

   Python: numgrid/__init__.py
     \
      \     Fortran: numgrid/numgrid.f90
       \   /
     C interface: numgrid/numgrid.h
         |
   implementation


Units
-----

Coordinates are in bohr.


Overview
--------

Grid computation is done per atom/basis type and proceeds in five steps:

-  Create atom
-  Get number of points (depends on basis set range)
-  Allocate memory to hold the grid
-  Compute grid on this atom in a molecular environment
-  Free atom and its memory

The Python interface takes care of the allocation and deallocation part
but the essential point is that memory management is happening on the
client side.

If you have many atom centers that have the same atom type and same
basis set, it will make sense to create only one atom object and then
reuse this object to compute the grid on all atoms with the same basis
type.

It is no problem to create several atom objects at the same time.


Python example
--------------

The Python interface is generated using
`CFFI <https://cffi.readthedocs.org>`__.

As an example let us generate a grid for the water molecule:

.. code:: python

   import numgrid

   radial_precision = 1.0e-12
   min_num_angular_points = 86
   max_num_angular_points = 302

   num_centers = 3
   proton_charges = [8, 1, 1]

   x_coordinates_bohr = [0.0, 1.43, -1.43]
   y_coordinates_bohr = [0.0, 0.0, 0.0]
   z_coordinates_bohr = [0.0, 1.1, 1.1]

   # cc-pVDZ basis
   alpha_max = [11720.0, 13.01, 13.01]  # O, H, H
   max_l_quantum_numbers = [2, 1, 1]  # O, H, H
   alpha_min = [[0.3023, 0.2753, 1.185],  # O
                [0.122, 0.727],  # H
                [0.122, 0.727]]  # H

   for center_index in range(num_centers):
       context = numgrid.new_atom_grid(radial_precision,
                                       min_num_angular_points,
                                       max_num_angular_points,
                                       proton_charges[center_index],
                                       alpha_max[center_index],
                                       max_l_quantum_numbers[center_index],
                                       alpha_min[center_index])

       num_points = numgrid.get_num_grid_points(context)

       # generate an atomic grid in the molecular environment
       x, y, z, w = numgrid.get_grid(context,
                                     num_centers,
                                     center_index,
                                     x_coordinates_bohr,
                                     y_coordinates_bohr,
                                     z_coordinates_bohr,
                                     proton_charges)

       num_radial_points = numgrid.get_num_radial_grid_points(context)

       # generate an isolated radial grid
       r, w = numgrid.get_radial_grid(context)

       numgrid.free_atom_grid(context)


   # generate an isolated angular grid
   x, y, z, w = numgrid.get_angular_grid(num_angular_grid_points=14)


Avoiding explicit exponent ranges
---------------------------------

Using the Python interface you can choose to not provide
explicit exponent ranges and instead specify the basis
set which is then fetched directly from
https://github.com/MolSSI-BSE/basis_set_exchange
using the wonderful
`MolSSI BSE <https://molssi-bse.github.io/basis_set_exchange/>`__:

.. code:: python

   context = numgrid.new_atom_grid_bse(radial_precision=1.0e-12,
                                       min_num_angular_points=86,
                                       max_num_angular_points=302,
                                       proton_charge=8,
                                       basis_set='cc-pVDZ')


Saving grid in Numpy format
---------------------------

The current API makes is relatively easy to export the computed grid in Numpy format.

In this example we save the radial grid positions and weights to two separate files
in Numpy format:

.. code:: python

   import numgrid
   import numpy as np

   # we assume the context is created
   # ...
   r, w = numgrid.get_radial_grid(context)

   np.save('radial_grid_r.npy', r)
   np.save('radial_grid_w.npy', w)


C API
-----

To see a real example, have a look at the `C++ test
case <test/test_explicit.cpp>`__.


Creating a new atom grid
~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: c

   context_t *numgrid_new_atom_grid(const double radial_precision,
                                    const int min_num_angular_points,
                                    const int max_num_angular_points,
                                    const int proton_charge,
                                    const double alpha_max,
                                    const int max_l_quantum_number,
                                    const double alpha_min[]);

The smaller the ``radial_precision``, the better grid.

For ``min_num_angular_points`` and ``max_num_angular_points``, see
“Angular grid” below.

``alpha_max`` is the steepest basis set exponent.

``alpha_min`` is an array of the size ``max_l_quantum_number`` + 1 and
holds the smallest exponents for each angular momentum. If an angular
momentum set is missing “in the middle”, provide 0.0. In other words,
imagine that you have a basis set which only contains *s* and *d*
functions and no *p* functions and let us assume that the most diffuse
*s* function has the exponent 0.1 and the most diffuse *d* function has
the exponent 0.2, then ``alpha_min`` would be an array of three numbers
holding {0.1, 0.0, 0.2}.


Get number of grid points on current atom
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following two functions are probably self-explaining. We need to
provide the context which refers to a specific atom object.

.. code:: c

   int numgrid_get_num_grid_points(const context_t *context);

   int numgrid_get_num_radial_grid_points(const context_t *context);


Get grid on current atom, scaled by Becke partitioning
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We assume that ``grid_x_bohr``, ``grid_y_bohr``, ``grid_z_bohr``, and
``grid_w`` are allocated by the caller and have the length that equals
the number of grid points.

``x_coordinates_bohr``, ``y_coordinates_bohr``, ``z_coordinates_bohr``,
and ``proton_charges`` refer to the molecular environment and have the
size ``num_centers``.

Using ``center_index`` we tell the code which of the atom centers is the
one we have computed the grid for.

.. code:: c

   void numgrid_get_grid(const context_t *context,
                         const int num_centers,
                         const int center_index,
                         const double x_coordinates_bohr[],
                         const double y_coordinates_bohr[],
                         const double z_coordinates_bohr[],
                         const int proton_charges[],
                         double grid_x_bohr[],
                         double grid_y_bohr[],
                         double grid_z_bohr[],
                         double grid_w[]);


Get radial grid on current atom
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We assume that ``radial_grid_r_bohr`` and ``radial_grid_w`` are
allocated by the caller and have both the length that equals the number
of radial grid points.

.. code:: c

   void numgrid_get_radial_grid(const context_t *context,
                                double radial_grid_r_bohr[],
                                double radial_grid_w[]);


Get angular grid
~~~~~~~~~~~~~~~~

This does not refer to any specific atom and does not require any
context.

``num_angular_grid_points`` has to be one of the many supported Lebedev
grids (see table on the bottom of this page) and the code will assume
that the grid arrays are allocated by the caller and have at least the
size ``num_angular_grid_points``.

.. code:: c

   void numgrid_get_angular_grid(const int num_angular_grid_points,
                                 double angular_grid_x_bohr[],
                                 double angular_grid_y_bohr[],
                                 double angular_grid_z_bohr[],
                                 double angular_grid_w[]);


Destroy the atom and deallocate all data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: c

   void numgrid_free_atom_grid(context_t *context);


Fortran API
-----------

Closely follows the C API. To see a real example, have a look at the
`Fortran test case <test/test_explicit.f90>`__.


Parallelization
===============

The design decision was to not parallelize the library but rather
parallelize over the atom/basis types by the caller. This simplifies
modularity and code reuse.


Space partitioning
==================

The molecular integration grid is generated from atom-centered grids by
scaling the grid weights according to the Becke partitioning scheme,
`JCP 88, 2547 (1988) <http://dx.doi.org/10.1063/1.454033>`__. The
default Becke hardness is 3.


Radial grid
===========

The radial grid is generated according to Lindh, Malmqvist, and
Gagliardi, `TCA 106, 178
(2001) <http://dx.doi.org/10.1007/s002140100263>`__.

The motivation for this choice is the nice feature of the above scheme
that the range of the radial grid is basis set dependent. The precision
can be tuned with one single radial precision parameter. The smaller the
radial precision, the better quality grid you obtain.

The basis set (more precisely the Gaussian primitives/exponents) are
used to generate the atomic radial grid range. This means that a more
diffuse basis set generates a more diffuse radial grid.

If you need a grid but you do not have a basis set or choose not to use
a specific one, then you can feed the library with a fantasy basis set
consisting of just two primitives. You can then adjust the range by
making the exponents more steep or more diffuse.


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


How to include numgrid in a CMake project
=========================================

There are multiple ways to achieve this. Here is how to include
the library using ``FetchContent``:

.. code:: cmake

  cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

  project(example LANGUAGES CXX)

  include(FetchContent)

  FetchContent_Declare(
    numgrid
    GIT_REPOSITORY https://github.com/dftlibs/numgrid.git
    GIT_TAG        e14bf969d68e7847f5e40f36816f61f245211a9b
  )

  FetchContent_GetProperties(numgrid)

  if(NOT numgrid_POPULATED)
    FetchContent_Populate(numgrid)
    add_subdirectory(
      ${numgrid_SOURCE_DIR}
      ${numgrid_BINARY_DIR}
      )
  endif()

  add_executable(example "")

  target_sources(
    example
    PRIVATE
      main.cpp
    )

  target_link_libraries(
    example
    PRIVATE
      numgrid-objects
    )
