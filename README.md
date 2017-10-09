[![Build Status](https://travis-ci.org/dftlibs/numgrid.svg?branch=master)](https://travis-ci.org/dftlibs/numgrid/builds)
[![Coverage Status](https://coveralls.io/repos/dftlibs/numgrid/badge.png?branch=master)](https://coveralls.io/r/dftlibs/numgrid?branch=master)
[![License](https://img.shields.io/badge/license-%20MPL--v2.0-blue.svg)](../master/LICENSE)

![alt text](https://github.com/dftlibs/numgrid/raw/master/img/truegrid.jpg "numgrid")

- [Build and test history](https://travis-ci.org/dftlibs/numgrid/builds)
- [Code coverage](https://coveralls.io/r/dftlibs/numgrid)
- Licensed under [MPL v2.0](../master/LICENSE) (except John Burkardt's Lebedev code which is redistributed under LGPL v3.0)
- Built with [Autocmake](https://github.com/coderefinery/autocmake)


# About

Numgrid is a library that produces numerical integration grid for molecules
based on atom coordinates, atom types, and basis set information.
Numgrid can be built with C, Fortran, and Python bindings.


# Who are the people behind this code?

## Authors

- Radovan Bast


## Contributors

- Roberto Di Remigio (OS X testing, streamlined Travis testing, better C++, error handling)

For a list of all the contributions see https://github.com/dftlibs/numgrid/contributors.


## Acknowledgements

- Simon Neville (reporting issues)
- Jaime Axel Rosal Sandberg (reporting issues)


## Would you like to contribute?

Yes please! Please follow [this excellent
guide](http://www.contribution-guide.org).  We do not require any formal
copyright assignment or contributor license agreement. Any contributions
intentionally sent upstream are presumed to be offered under terms of the
Mozilla Public License Version 2.0.


# Requirements

- CMake
- C and C++ compiler
- Fortran compiler (to build the optional Fortran interface)
- [CFFI](https://cffi.readthedocs.org) (to access the optional Python interface)
- [pytest](http://pytest.org) (to test the optional Python interface)


# Installation

## Installing via pip

```
pip install git+https://github.com/dftlibs/numgrid@master
```


## Building and testing from sources

Fetch the code:

```
git clone https://github.com/dftlibs/numgrid.git
```

Install Python dependencies (optional):

```
virtualenv venv
source venv/bin/activate
pip install -r requirements.txt
```

Build the code:

```
cd numgrid
./setup --fc=gfortran --cc=gcc --cxx=g++
cd build
make
make test
```

Test the Python interface (optional):

```
NUMGRID_BUILD_DIR=. PYTHONPATH=../api py.test -s -vv ../test/test.py
```


# API

The library provides a context-aware C interface. In addition it also provides a Fortran
and Python interfaces as thin layers on top of the C interface:

```
Python: api/numgrid.py
  \
   \     Fortran: api/numgrid.f90
    \   /
  C interface: api/numgrid.h
      |
implementation
```


## What changed since v0.5?

### Grid memory management is now client-side

This was done to simplify memory management and avoid memory leaks and strange
effects.  The client can now query the number of grid points before computing
the grid for a certain atom type. Sounds cumbersome but is not a problem in practice.
For the Python interface this is not a problem at all since it takes care of that.


### Compute one center at a time

Motivation was to simplify code and to make it possible to pre-compute a grid
for a certain atom/basis type.  This also means that the code can be optimized
and parallelized on the client side.


### Full basis set does not need to be provided

Great simplification. All that is needed now is the steepest exponent and a set
of smallest exponents for each angular momentum.


## Obtain the API version

```c
char *numgrid_get_version();
```


## Creating and destroying contexts

Create a new context:

```c
context_t *numgrid_new_context();
```

Destroy the context and deallocates all data:

```c
void numgrid_free_context(context_t *context);
```

You can keep several contexts alive at the same time.


## Generating the grid

Generate grid and hold it in memory for the lifetime of the context (returns 0 if the call
completed without errors):

```c
int numgrid_generate_grid(context_t *context,
                          const double radial_precision,
                          const int    min_num_angular_points,
                          const int    max_num_angular_points,
                          const int    num_centers,
                          const double center_coordinates[],
                          const int    center_elements[],
                          const int    num_outer_centers,
                          const double outer_center_coordinates[],
                          const int    outer_center_elements[],
                          const int    num_shells,
                          const int    shell_centers[],
                          const int    shell_l_quantum_numbers[],
                          const int    shell_num_primitives[],
                          const double primitive_exponents[]);
```


## How to query the grid

Get number of grid points:

```c
int numgrid_get_num_points(const context_t *context);
```

Get the pointer to the memory which holds the grid:

```c
double *numgrid_get_grid(const context_t *context);
```

The grid is saved in a one-dimensional array with the following ordering:

```
point 1, x coordinate
point 1, y coordinate
point 1, z coordinate
point 1, weight
point 2, x coordinate
point 2, y coordinate
point 2, z coordinate
point 2, weight
...
point N, x coordinate
point N, y coordinate
point N, z coordinate
point N, weight
```


## Units

`center_coordinates` are understood to be in bohr.


## Python interface

The Python interface is generated using [CFFI](https://cffi.readthedocs.org).

As an example let us generate a grid for the water molecule:

```python
import numgrid

radial_precision = 1.0e-12
min_num_angular_points = 86
max_num_angular_points = 302

num_centers = 3
proton_charges = [8, 1, 1]

x_coordinates_au = [0.0, 1.43, -1.43]
y_coordinates_au = [0.0, 0.0, 0.0]
z_coordinates_au = [0.0, 1.1, 1.1]

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
                                  x_coordinates_au,
                                  y_coordinates_au,
                                  z_coordinates_au,
                                  proton_charges)

    num_radial_points = numgrid.get_num_radial_grid_points(context)

    # generate an isolated radial grid
    r, w = numgrid.get_radial_grid(context)

    numgrid.free_atom_grid(context)


# generate an isolated angular grid
x, y, z, w = numgrid.get_angular_grid(num_angular_grid_points=14)
```


# Parallelization

The design decision was to not parallelize the library but rather parallelize
over the atom/basis types by the caller. This simplifies modularity and code
reuse.


# Space partitioning

The molecular integration grid is generated from atom-centered
grids by scaling the grid weights according
to the Becke partitioning scheme,
[JCP 88, 2547 (1988)](http://dx.doi.org/10.1063/1.454033).
The default Becke hardness is 3.


# Radial grid

The radial grid is generated according to Lindh, Malmqvist, and Gagliardi,
[TCA 106, 178 (2001)](http://dx.doi.org/10.1007/s002140100263).

The motivation for this choice is the nice feature of the above scheme that the
range of the radial grid is basis set dependent. The precision can be tuned
with one single radial precision parameter.
The smaller the radial precision, the better quality grid you obtain.

The basis set (more precisely the Gaussian primitives/exponents) are used to
generate the atomic radial grid range. This means that a more diffuse basis set
generates a more diffuse radial grid.

If you need a grid but you do not have a basis set or choose not to use a
specific one, then you can feed the library with a fantasy basis set consisting
of just two primitives. You can then adjust the range by making the exponents
more steep or more diffuse.


# Angular grid

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
{6,    14,   26,   38,   50,   74,   86,   110,  146,
 170,  194,  230,  266,  302,  350,  434,  590,  770,
 974,  1202, 1454, 1730, 2030, 2354, 2702, 3074, 3470,
 3890, 4334, 4802, 5294, 5810}
```

Taking the same number for the minimum and maximum number of angular points
switches off pruning.
