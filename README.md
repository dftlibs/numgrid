[![Build Status](https://travis-ci.org/bast/numgrid.svg?branch=master)](https://travis-ci.org/bast/numgrid/builds)
[![Coverage Status](https://coveralls.io/repos/bast/numgrid/badge.png?branch=master)](https://coveralls.io/r/bast/numgrid?branch=master)

![alt text](https://github.com/bast/numgrid/raw/master/img/truegrid.jpg "numgrid")

- [Build and test history](https://travis-ci.org/bast/numgrid/builds)
- [Code coverage](https://coveralls.io/r/bast/numgrid)
- Licensed under [BSD-3](../master/LICENSE) (except John Burkardt's Lebedev code which is redistributed under LGPLv3)
- Built with [Autocmake](https://github.com/scisoft/autocmake)


# About

Numgrid is a library that produces numerical integration grid for molecules
based on atom coordinates, atom types, and basis set information.
Numgrid can be built with C, Fortran, and Python bindings.


# Contributing

Yes please! Please follow [this excellent
guide](http://www.contribution-guide.org).  We do not require any formal
copyright assignment or contributor license agreement. Any contributions
intentionally sent upstream are presumed to be offered under terms of the
OSI-approved BSD 3-clause License.


# Requirements

- CMake
- C and C++ compiler
- Fortran compiler (to build the optional Fortran interface)
- [CFFI](https://cffi.readthedocs.org) (to access the optional Python interface)
- [py.test](http://pytest.org) (to test the optional Python interface)


# Building and testing

```
git clone --recursive https://github.com/bast/numgrid.git
cd numgrid
./setup --fc=gfortran --cc=gcc --cxx=g++
cd build
make
make test
```


# Parallelization

The design decision was to not parallelize the library but rather parallelize
over the generated points by the caller. This simplifies modularity and code
reuse.  See also the section about "Distributed computation using 'outer
centers'".


# Outer centers

For very large molecules the caller may not want to generate the entire grid at
once but perhaps only generate center by center or fragment by fragment
(however taking Becke partitioning into account) and possibly keep the centers
or fragments on different parallel tasks or threads. This can be achieved by
using 'outer centers'. The 'outer centers' with influence the active centers in
the generation of the Becke partitioning weights but will not carry any grid
points themselves. This can be used for multi-scale computations.

Another use-case for 'outer centers' is to create a grid with
varying grid quality across the system/molecule.


# Why is the grid basis set dependent?

The basis set (more precisely the Gaussian primitives/exponents) are used to
generate the atomic radial grid range. This means that a more diffuse basis set
generates a more diffuse radial grid.

If you need a grid but you do not have a basis set or choose not to use a
specific one, then you can feed the library with a fantasy basis set consisting
of just two primitives. You can then adjust the range by making the exponents
more steep or more diffuse.


# API

Use the [source](../master/api/numgrid.h) Luke!


# Python interface

The Python interface is generated using [CFFI](https://cffi.readthedocs.org).

As an example let us generate a grid for the water molecule:

```python
from numgrid import lib

radial_precision = 1.0e-12
min_num_angular_points = 86
max_num_angular_points = 302

num_centers = 3
center_coordinates = [0.0000e+00,
                      0.0000e+00,
                      0.0000e+00,
                      5.5168e-01,
                      7.7340e-01,
                      0.0000e+00,
                      5.5168e-01,
                      -7.734e-01,
                      0.0000e+00]
center_elements = [8, 1, 1]

num_outer_centers = 0
outer_center_coordinates = []
outer_center_elements = []

num_shells = 12
shell_centers = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3]
shell_l_quantum_numbers = [0, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0, 1]
shell_num_primitives = [9, 9, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1]

primitive_exponents = [1.172e+04,
                       1.759e+03,
                       4.008e+02,
                       ...  # we skip many numbers here for brevity
                       7.270e-01]

context = lib.numgrid_new()

ierr = lib.numgrid_generate(context,
                            radial_precision,
                            min_num_angular_points,
                            max_num_angular_points,
                            num_centers,
                            center_coordinates,
                            center_elements,
                            num_outer_centers,
                            outer_center_coordinates,
                            outer_center_elements,
                            num_shells,
                            shell_centers,
                            shell_l_quantum_numbers,
                            shell_num_primitives,
                            primitive_exponents)

num_points = lib.numgrid_get_num_points(context)

grid = lib.numgrid_get_grid(context)

lib.numgrid_free(context)
```


## Testing the Python interface

```
py.test test/test.py <build_dir>
```

# Integration grid

The molecular integration grid is generated from atom-centered
grids by scaling the grid weights according
to the Becke partitioning scheme,
[JCP 88, 2547 (1988)](http://dx.doi.org/10.1063/1.454033).
The default Becke hardness is 3.

Each atomic grid has radial shells with corresponding radial weights.  Each of
the radial shells carries an angular grid with a certain number of angular
points. The generating schemes for the radial and angular grids are outlined
below.


## Radial grid

The radial grid is generated according to Lindh, Malmqvist, and Gagliardi,
[TCA 106, 178 (2001)](http://dx.doi.org/10.1007/s002140100263).

The motivation for this choice is the nice feature of the above scheme that the
range of the radial grid is basis set dependent. The precision can be tuned
with one single radial precision parameter.
The smaller the radial precision, the better.


## Angular grid

The angular grid is generated according to
Lebedev and Laikov
[A quadrature formula for the sphere of the 131st
algebraic order of accuracy,
Russian Academy of Sciences Doklady Mathematics,
Volume 59, Number 3, 1999, pages 477-481].

The angular grid is pruned.
The pruning is a primitive linear interpolation between the minimum number and
the maximum number of angular points per radial shell.
The maximum number is reached at 0.2 times the Bragg radius of the center.

The higher the values for minimum and maximum number of angular points, the better.

For the minimum and maximum number of angular points the code will use the following
table and select the closest number with at least the desired precision:

```
  {6,   14,   26,   38,   50,   74,   86,  110,  146,  170,
 194,  230,  266,  302,  350,  434,  590,  770,  974, 1202,
1454, 1730, 2030, 2354, 2702, 3074, 3470, 3890, 4334, 4802,
4934, 5294, 5810}
```

Taking the same number for the minimum and maximum number of angular points
switches off pruning.
